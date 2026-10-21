/*
 * A079471  Fixed points of reversed binary words in reversed lexicographic
 *          order.
 * https://oeis.org/A079471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 1093, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow2, trailing_zeros_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A079471: integers n such that popcount(n) == 2^i where i is the
   index of the lowest set bit.  0 is output as the first term (a(0) = 0).
   The function generates subsequent terms by scanning successive integers
   and testing the property. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/pow2.h"
#include "../utils/trailing_zeros_seq_t.h"

/* Simple popcount for seq_t.  Works for built-in or arbitrary precision
   integer types that support bitwise operations and shift. */
static inline int popcount_seq_t(seq_t x)
{
    int cnt = 0;
    while (x) {
        cnt += (int)(x & 1);
        x >>= 1;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* array of computed terms */
    static size_t cache_len = 0;         /* number of elements in cache */
    static size_t cache_cap = 0;         /* allocated capacity of cache */
    static seq_t next_candidate = 1;     /* next integer to test */

    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;            /* allocation failure */
        cache[0] = 0;                    /* a(0) = 0 */
        cache_len = 1;
        next_candidate = 1;
    }

    /* Extend cache until it contains at least n+1 elements */
    while ((idx_t)cache_len <= n) {
        seq_t val = next_candidate;
        next_candidate = val + 1;

        if (val == 0) {                  /* skip 0; already stored */
            continue;
        }

        unsigned tz = trailing_zeros_seq_t(val);
        int bc   = popcount_seq_t(val);
        seq_t p2 = pow2(tz);              /* 2^tz */

        if ((seq_t)bc == p2) {
            /* Ensure enough space */
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) return 0;        /* allocation failure */
                cache = tmp;
            }
            cache[cache_len++] = val;     /* store new term */
        }
    }

    return cache[n];
}
