/*
 * A107813  a(1) = prime(13), for n >= 2, a(n) is the smallest prime not
 *          previously used which contains a digit from a(n-1).
 * https://oeis.org/A107813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is defined by a(1)=prime(13)=41, for n≥2 a(n) is the
 *            smallest unused prime that contains a digit of a(n-1)
 * Range:     u8 n <= 54, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   contains_mask
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Author: ChatGPT */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/contains_mask.h"

/* Forward declaration of the required function. */
seq_t a(idx_t n);

/* Internal helpers */

/* Deterministic Miller–Rabin for 64‑bit integers. */
static uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod) {
    return (uint64_t)((__uint128_t)a * b % mod);
}
static uint64_t pow_mod(uint64_t a, uint64_t e, uint64_t mod) {
    uint64_t r = 1 % mod;
    while (e) {
        if (e & 1) r = mul_mod(r, a, mod);
        a = mul_mod(a, a, mod);
        e >>= 1;
    }
    return r;
}
static bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    static const uint64_t small[] = {2,3,5,7,11,13,17};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        if (n % small[i] == 0) return n == small[i];
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        uint64_t a = small[i];
        if (a % n == 0) continue;
        uint64_t x = pow_mod(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Return a bitmask of the decimal digits present in x. */
static uint16_t digit_mask(uint64_t x) {
    uint16_t m = 0;
    do {
        m |= 1U << (x % 10);
        x /= 10;
    } while (x);
    return m;
}

/* Sieve of Eratosthenes up to LIMIT, storing primes in primes[]. */
#define LIMIT 400000          /* Safe upper bound for n≤10000 */
static uint64_t *primes = NULL;
static size_t primes_size = 0;
static bool *used_primes = NULL;

static void init_primes(void) {
    bool *is_comp = (bool*)calloc(LIMIT + 1, sizeof(bool));
    if (!is_comp) exit(1);
    for (uint64_t i = 2; i * i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            for (uint64_t j = i * i; j <= LIMIT; j += i) is_comp[j] = true;
        }
    }
    primes = (uint64_t*)malloc((LIMIT / 10) * sizeof(uint64_t));  /* rough estimate */
    if (!primes) exit(1);
    primes_size = 0;
    for (uint64_t i = 2; i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            if (primes_size % 64 == 0) {
                primes = (uint64_t*)realloc(primes, (primes_size / 64 + 1) * 64 * sizeof(uint64_t));
                if (!primes) exit(1);
            }
            primes[primes_size++] = i;
        }
    }
    free(is_comp);
    used_primes = (bool*)calloc(primes_size, sizeof(bool));
    if (!used_primes) exit(1);
}

/* Sequence storage */
static uint64_t *seq_arr = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

static void ensure_seq(size_t i) {
    if (i < seq_cap) return;
    while (seq_cap <= i) seq_cap = seq_cap ? seq_cap * 2 : 64;
    seq_arr = (uint64_t*)realloc(seq_arr, seq_cap * sizeof(uint64_t));
    if (!seq_arr) exit(1);
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* Undefined, return 0 */
    /* Initialise once */
    if (!primes) init_primes();

    while (seq_len < (size_t)n) {
        if (seq_len == 0) {
            /* a(1) = 41 */
            uint64_t val = 41;
            /* find index of 41 in primes list */
            for (size_t i = 0; i < primes_size; ++i) {
                if (primes[i] == val) {
                    used_primes[i] = true;
                    break;
                }
            }
            ensure_seq(seq_len);
            seq_arr[seq_len++] = val;
            continue;
        }
        uint64_t prev = seq_arr[seq_len - 1];
        uint16_t mask = digit_mask(prev);
        uint64_t chosen = 0;
        for (size_t i = 0; i < primes_size; ++i) {
            if (!used_primes[i] && contains_mask(primes[i], mask)) {
                chosen = primes[i];
                used_primes[i] = true;
                break;
            }
        }
        /* In normal use the chosen value is found within LIMIT. */
        ensure_seq(seq_len);
        seq_arr[seq_len++] = chosen;
    }
    return (seq_t)seq_arr[n - 1];
}
