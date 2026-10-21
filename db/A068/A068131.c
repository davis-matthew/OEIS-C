/*
 * A068131  Triangular numbers with sum of digits = 21.
 * https://oeis.org/A068131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A068131: Triangular numbers whose digit sum equals 21 */
/* Computation: iterate triangular numbers T_m = m(m+1)/2 and collect
   those with digit_sum = 21.  The nth collected value is returned. */

#include <stdlib.h>
#include "../utils/digit_sum.h"   /* digit_sum(seq_t) */

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache      = NULL; /* array of collected triangular numbers   */
    static size_t cache_cap = 0;     /* current capacity of cache              */
    static idx_t  max_computed = 0;  /* number of terms already stored         */
    static idx_t  next_m      = 0;   /* last m processed (so next is next_m+1) */

    /* lazy allocation of initial cache */
    if (cache == NULL) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* out of memory – abort */
    }

    /* generate terms until we have at least n of them */
    while (max_computed < n) {
        ++next_m;

        /* compute T_m in a wide type to avoid overflow, then truncate to seq_t */
        __int128 t = (__int128)next_m * (next_m + 1) / 2;
        seq_t tri = (seq_t)t;

        /* keep only those with digit sum 21 */
        if (digit_sum(tri) == (seq_t)21) {
            /* grow cache if necessary */
            if (max_computed == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0; /* out of memory – abort */
                cache   = tmp;
                cache_cap = new_cap;
            }
            cache[max_computed++] = tri;
        }
    }

    return cache[n - 1];
}
