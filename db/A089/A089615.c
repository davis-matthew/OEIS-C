/*
 * A089615  Number of primes between n^2 and (n+1/5)^2.
 * https://oeis.org/A089615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of primes p with n^2 < p < (n+1/5)^2
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Utility functions for an odd‑only bitset ---------- */
static inline bool bit_get(const uint8_t *bits, uint32_t idx) {
    return (bits[idx >> 3] >> (idx & 7)) & 1;
}
static inline void bit_set(uint8_t *bits, uint32_t idx) {
    bits[idx >> 3] |= (1 << (idx & 7));
}
static inline void bit_clear(uint8_t *bits, uint32_t idx) {
    bits[idx >> 3] &= ~(1 << (idx & 7));
}

/* ---------- Sieve of Eratosthenes for odd numbers only ---------- */
static bool sieve_inited = false;
static uint32_t sieve_limit = 0;   /* maximum number sieved (inclusive)   */
static uint8_t *sieve_bits = NULL; /* bitset: bit i represents number 3+2*i */

static void init_sieve(uint32_t limit) {
    if (sieve_inited && limit <= sieve_limit)
        return;   /* already enough */

    /* allocate new bitset */
    uint32_t bits_needed = (limit >= 3) ? ((limit - 3) >> 1) + 1 : 0;
    size_t arr_bytes = (bits_needed + 7) >> 3;
    uint8_t *new_bits = (uint8_t *)calloc(arr_bytes, 1);
    if (!new_bits)
        return;   /* catastrophic out‑of‑memory; just leave empty */

    /* mark all odd numbers as prime initially */
    for (uint32_t i = 0; i < bits_needed; ++i)
        bit_set(new_bits, i);

    /* sieve */
    for (uint32_t i = 0; ; ++i) {
        uint32_t p = 3 + 2 * i;
        if ((uint64_t)p * (uint64_t)p > limit)
            break;
        if (!bit_get(new_bits, i))
            continue;
        /* cross out multiples of p: start at p*p, step 2p */
        for (uint64_t m = (uint64_t)p * (uint64_t)p; m <= limit; m += (uint64_t)p * 2ULL) {
            uint32_t idx = (uint32_t)((m - 3) >> 1);
            bit_clear(new_bits, idx);
        }
    }

    /* free old data and commit new one */
    if (sieve_bits)
        free(sieve_bits);
    sieve_bits   = new_bits;
    sieve_limit  = limit;
    sieve_inited = true;
}

/* ---------- Simple prime test using the bitset ---------- */
static inline bool is_prime_uint32(uint32_t x) {
    if (x < 2)
        return false;
    if (x == 2)
        return true;
    if ((x & 1) == 0)
        return false;
    if (sieve_inited && x <= sieve_limit) {
        uint32_t idx = (x - 3) >> 1;
        return bit_get(sieve_bits, idx);
    }
    /* fallback trial division (never used for the tested range) */
    for (uint32_t p = 3; (uint64_t)p * (uint64_t)p <= x; p += 2)
        if (x % p == 0)
            return false;
    return true;
}

/* ---------- Prime test for large numbers (used only in the \
 *            unlikely event the range exceeds 32‑bit limits) ---------- */
#include "../utils/is_prime_big.h"      /* provides bool is_prime_big(seq_t) */
static inline bool is_prime_big_wrapper(uint64_t v) {
    return is_prime_big((seq_t)v);
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n) {
    if (n < 1)
        return 0;

    /* compute bounds: low = n^2, high = floor((n+1/5)^2) */
    uint64_t nn  = (uint64_t)n;
    uint64_t low = nn * nn;
    uint64_t tmp = 5 * nn + 1ULL;
    uint64_t high = (tmp * tmp) / 25ULL;   /* floor((n+1/5)^2) */

    if (high <= low)
        return 0;   /* empty interval */

    /* if the interval is inside 32‑bit range, use fast sieve */
    if (high <= UINT32_MAX) {
        uint32_t ulow  = (uint32_t)low;
        uint32_t uhigh = (uint32_t)high;

        init_sieve(uhigh);   /* ensure sieve covers the high bound */

        seq_t cnt = 0;
        for (uint32_t x = ulow + 1; x <= uhigh; ++x) {
            if (is_prime_uint32(x))
                ++cnt;
        }
        return cnt;
    }

    /* otherwise, fall back to prime test for each integer */
    seq_t cnt = 0;
    for (uint64_t x = low + 1; x <= high; ++x) {
        if (is_prime_big_wrapper(x))
            ++cnt;
    }
    return cnt;
}
