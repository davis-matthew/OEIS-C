/*
 * A106548  Perfect squares n^2 which are both the sum and the difference of
 *          two primes (otherwise 0).
 * https://oeis.org/A106548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^2 if n^2 is both the sum and the difference of two
 *            primes, otherwise 0
 * Range:     u8 n <= 15, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static const uint32_t PRIME_CAND_MAX = 1000000;  // prime candidates for difference check

/* Miller–Rabin primality test for 32‑bit integers. */
static uint64_t mod_mul64(uint64_t a, uint64_t b, uint64_t mod) {
    return (a * b) % mod;
}

static uint64_t mod_pow64(uint64_t a, uint64_t d, uint64_t mod) {
    uint64_t result = 1;
    a %= mod;
    while (d) {
        if (d & 1) result = mod_mul64(result, a, mod);
        a = mod_mul64(a, a, mod);
        d >>= 1;
    }
    return result;
}

static bool is_prime32(uint32_t n) {
    if (n < 2) return false;
    static const uint32_t small_primes[] = {
        2,3,5,7,11,13,17,19,23,29,31,37,0
    };
    for (int i = 0; small_primes[i]; ++i) {
        uint32_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    /* Miller–Rabin bases {2,7,61} are deterministic for n < 2^32 */
    uint32_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    const uint32_t bases[] = {2,7,61,0};
    for (int i = 0; bases[i]; ++i) {
        uint32_t a = bases[i];
        if (a % n == 0) continue;
        uint64_t x = mod_pow64(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool composite = true;
        for (uint32_t r = 1; r < s; ++r) {
            x = mod_mul64(x, x, n);
            if (x == n-1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* Pre‑sieve small primes up to PRIME_CAND_MAX once. */
static void init_primes(uint32_t **primes, size_t *count) {
    static bool initialized = false;
    if (initialized) return;
    size_t n = PRIME_CAND_MAX + 1;
    bool *is_composite = (bool*)calloc(n, sizeof(bool));
    if (!is_composite) exit(EXIT_FAILURE);
    *count = 0;
    for (uint32_t i = 2; i <= PRIME_CAND_MAX; ++i) {
        if (!is_composite[i]) {
            ++*count;
            if ((uint64_t)i * i <= PRIME_CAND_MAX)
                for (uint32_t j = i * i; j <= PRIME_CAND_MAX; j += i)
                    is_composite[j] = true;
        }
    }
    *primes = (uint32_t*)malloc(*count * sizeof(uint32_t));
    size_t idx = 0;
    for (uint32_t i = 2; i <= PRIME_CAND_MAX; ++i)
        if (!is_composite[i]) (*primes)[idx++] = i;
    free(is_composite);
    initialized = true;
}

static uint32_t *PRIMES = NULL;
static size_t PRIMES_COUNT = 0;
static void ensure_primes(void) {
    if (!PRIMES) init_primes(&PRIMES, &PRIMES_COUNT);
}

static bool sum_of_two_primes(uint32_t s) {
    if (s == 2) return true;          // 2 = 1 + 1 (not prime) but not used
    if ((s & 1) == 0) {
        return s > 2;                 /* Goldbach verified up to 4e18 – safe here */
    } else {
        return is_prime32(s - 2);      /* 2 + (s-2) */
    }
}

static bool diff_of_two_primes(uint32_t s) {
    if ((s & 1) == 1) {               /* odd: must be of form p-2 */
        return is_prime32(s + 2);
    } else {
        /* look for small prime p such that p and p+s are primes */
        ensure_primes();
        for (size_t i = 0; i < PRIMES_COUNT; ++i) {
            uint32_t p = PRIMES[i];
            if (p + (uint64_t)s > 0xFFFFFFFFu) break; /* stays within 32‑bit */
            if (is_prime32(p + (uint32_t)s)) return true;
        }
        return false; /* none found among precomputed primes */
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;      /* offset is 1, behaviour below offset is irrelevant */
    seq_t sq = (seq_t)n * (seq_t)n;
    uint32_t sq32 = (uint32_t)sq;   /* n up to 10000, fits in 32‑bit */
    if (sum_of_two_primes(sq32) && diff_of_two_primes(sq32))
        return sq;
    return (seq_t)0;
}
