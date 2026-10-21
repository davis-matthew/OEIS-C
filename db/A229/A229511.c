/*
 * A229511  Numbers k whose arithmetic derivative (k') is a triangular number.
 * https://oeis.org/A229511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 64, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A229511
   Description: Numbers k whose arithmetic derivative k' is a triangular number.
   The function a(idx_t n) returns the n‑th such integer (a(0)=0).       */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/floor_sqrt.h"

static inline bool is_triangular(seq_t x)
{
    /* x is triangular iff 8*x+1 is a perfect square */
    seq_t disc = (seq_t)8 * x + 1;
    seq_t s = floor_sqrt(disc);
    return s * s == disc;
}

seq_t a(idx_t n)
{
    /* Static cache of found values */
    static seq_t *cache   = NULL;   /* sequence values */
    static idx_t cache_len = 0;     /* number of values stored */
    static size_t cache_cap = 0;    /* capacity of the cache array */
    static uint64_t next_k = 0;     /* next integer to test */

    /* If we already have a(n), return it immediately */
    if (cache_len > n)
        return cache[n];

    /* Grow the cache until it contains a(n) */
    while (cache_len <= n)
    {
        seq_t deriv = 0;

        /* arithmetic derivative of next_k */
        if (next_k <= 1ULL) {
            deriv = 0;                /* 0' = 0, 1' = 0 */
        } else {
            uint64_t num = (uint64_t)next_k;
            uint64_t primes[64];
            uint64_t exps[64];
            size_t cnt = 0;

            factor_u64(num, primes, exps, &cnt);
            for (size_t i = 0; i < cnt; ++i) {
                deriv += (seq_t)exps[i] * (seq_t)(num / primes[i]);
            }
        }

        /* If the derivative is triangular, store the candidate */
        if (is_triangular(deriv))
        {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 32;
                cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_k;
        }

        ++next_k;   /* move to next integer */
    }

    return cache[n];
}
