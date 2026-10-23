/*
 * A101444  Numbers k such that (9973*k + 10007) is a prime.
 * https://oeis.org/A101444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 3367, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 7606, big any n
 * Helpers:   mul_mod64, pow_mod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A101444: a(n) = the n‑th integer k for which 9973·k + 10007 is prime */
/* The function is internally memoised so calls in any order are fast. */
/* It uses a deterministic Miller–Rabin test for 64‑bit integers. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mul_mod64.h"
#include "../utils/pow_mod64.h"

/* deterministic bases for 64‑bit numbers */
static const uint64_t mr_bases[] = {2ULL, 325ULL, 9375ULL, 28178ULL,
                                    450775ULL, 9780504ULL, 1795265022ULL};

static inline int is_prime_uint64(uint64_t n)
{
    if (n < 2) return 0;
    static const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,
                                            31,37,41,43,47,53,59,61,67,71,
                                            73,79,83,89,97};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return 1;
        if (n % p == 0) return 0;
    }

    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    for (size_t i = 0; i < sizeof(mr_bases)/sizeof(mr_bases[0]); ++i) {
        uint64_t a = mr_bases[i] % n;
        if (a <= 1) continue;
        uint64_t x = pow_mod64(a, d, n);
        if (x == 1 || x == n-1) continue;
        int composite = 1;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod64(x, x, n);
            if (x == n-1) { composite = 0; break; }
        }
        if (composite) return 0;
    }
    return 1;
}

/* ---------- memoised sequence ---------------------------------------- */
static seq_t *cache      = NULL;
static idx_t  cache_cnt  = 0;
static idx_t  cache_cap  = 0;
static idx_t  k_next     = 0;   /* next k to test */

/* External function required by the build. */
seq_t a(idx_t n)
{
    /* Return cached value if already known */
    if (n <= cache_cnt)
        return cache[n-1];

    /* Expand cache capacity if necessary */
    if (cache_cnt == cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 8;
        seq_t *new_arr = (seq_t *)malloc(sizeof(seq_t) * new_cap);
        if (new_arr == NULL) abort();              /* OOM */
        if (cache != NULL) {
            memcpy(new_arr, cache, sizeof(seq_t) * cache_cnt);
            free(cache);
        }
        cache     = new_arr;
        cache_cap = new_cap;
    }

    /* Generate new terms until *n* terms are available */
    while (cache_cnt < n) {
        uint64_t cand = 9973ULL * (uint64_t)k_next + 10007ULL;
        if (is_prime_uint64(cand)) {
            cache[cache_cnt++] = (seq_t)k_next;
        }
        ++k_next;
    }

    return cache[n-1];
}
