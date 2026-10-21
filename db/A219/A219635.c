/*
 * A219635  a(n) is the smallest number > a(n-1) such that 1 + a(1)^3 + a(2)^3
 *          + ... + a(n)^3 is a prime.
 * https://oeis.org/A219635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   mod_mul64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mod_mul64.h"

/* Computes OEIS A219635:  a(n) is the smallest number > a(n-1)
   such that 1 + Σ_{k=1..n} a(k)^3 is prime. */
static seq_t *cache_a   = NULL;   /* stored a(n) values (1‑based) */
static uint64_t *cache_sum = NULL;/* accumulated sum 1+Σ a(k)^3,  uint64_t is large enough for n≤10000 */
static idx_t cache_len = 0;       /* number of terms already computed */
static idx_t cache_cap = 0;       /* allocated capacity of the arrays */

/* Ensure capacity for at least `need` elements */
static void ensure_capacity(idx_t need) {
    if (need <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 1;
    if (new_cap < need) new_cap = need;

    seq_t *new_a = (seq_t *)realloc(cache_a, new_cap * sizeof(seq_t));
    uint64_t *new_s = (uint64_t *)realloc(cache_sum, new_cap * sizeof(uint64_t));
    if (!new_a || !new_s) abort();

    cache_a = new_a;
    cache_sum = new_s;
    cache_cap = new_cap;
}

/* 64‑bit modular multiplication using the built‑in 128‑bit type */

/* 64‑bit modular exponentiation */
static uint64_t mod_pow64(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1) result = mod_mul64(result, b, mod);
        b = mod_mul64(b, b, mod);
        exp >>= 1;
    }
    return result;
}

/* Deterministic Miller‑Rabin for 64‑bit integers */
static bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    static const uint64_t SMALL_PRIMES[] = {
        2,3,5,7,11,13,17,19,23,29,31,37
    };
    for (size_t i = 0; i < sizeof(SMALL_PRIMES)/sizeof(SMALL_PRIMES[0]); ++i) {
        uint64_t p = SMALL_PRIMES[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    uint64_t d = n - 1;
    unsigned int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    static const uint32_t B[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37
    };
    for (size_t i = 0; i < sizeof(B)/sizeof(B[0]); ++i) {
        uint64_t a = B[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow64(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool composite = true;
        for (unsigned r = 1; r < s; ++r) {
            x = mod_mul64(x, x, n);
            if (x == n-1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* Main function required by the contest */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* undefined for n=0 */

    while (cache_len < n) {
        ensure_capacity(cache_len + 1);

        if (cache_len == 0) {            /* a(1) = 1, sum = 2 */
            cache_a[0]   = (seq_t)1;
            cache_sum[0] = 2ULL;
            ++cache_len;
            continue;
        }

        seq_t prev_a = cache_a[cache_len - 1];
        uint64_t prev_sum = cache_sum[cache_len - 1];

        idx_t cand = (idx_t)(prev_a) + 1;
        while (1) {
            uint64_t c   = (uint64_t)cand;
            uint64_t c3  = c * c * c;
            uint64_t sum = prev_sum + c3;

            if (is_prime_uint64(sum)) {
                cache_a[cache_len]   = (seq_t)cand;
                cache_sum[cache_len] = sum;
                break;
            }
            ++cand;
        }
        ++cache_len;
    }

    return cache_a[n - 1];
}
