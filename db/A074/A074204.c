/*
 * A074204  Numbers k such that the number of 1's in the binary representation
 *          of k divides 2^k+1.
 * https://oeis.org/A074204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 3479, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow2_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The nth term is the n‑th integer k such that popcount(k) divides 2^k + 1. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow2_mod.h"

/* count 1‑bits of a 64‑bit integer */
static inline unsigned popcount_u64(uint64_t x)
{
    return __builtin_popcountll(x);
}

/* cache of already found terms */
static seq_t *cache   = NULL;   /* array of seq_t */
static size_t cache_len = 0;    /* number of terms stored */
static idx_t   next_k    = 1;    /* next integer to test after the last found term */

seq_t a(idx_t n)
{
    /* n is 1‑based; sequence starts at a(1) */
    if (n == 0) return 0;

    /* grow cache until it contains at least n terms */
    while (cache_len < n) {
        while (1) {
            unsigned w = popcount_u64((uint64_t)next_k);
            uint64_t mod = pow2_mod((uint64_t)next_k, w);
            if ((mod + 1U) % w == 0U) {           /* condition satisfied */
                /* extend the cache */
                size_t new_len = cache_len + 1;
                void *tmp = realloc(cache, new_len * sizeof(seq_t));
                if (!tmp) abort();      /* allocation failure */
                cache = (seq_t*)tmp;
                cache[cache_len] = (seq_t)next_k;
                cache_len = new_len;
                ++next_k;
                break;
            }
            ++next_k;
        }
    }

    return cache[n - 1];
}
