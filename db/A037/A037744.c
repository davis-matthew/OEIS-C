/*
 * A037744  Base 4 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,3,0,1.
 * https://oeis.org/A037744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1) = 2,  a(2) = 11,  a(3) = 44,  a(4) = 177,  a(5) = 710
   a(n) = 4*a(n-1) + a(n-4) - 4*a(n-5)   for n ≥ 6
   (indices in the code are zero‑based: cache[0] == a(1)) */

#include <stdlib.h>

/* Static cache of computed terms */
static seq_t *cache = NULL;
static size_t cached_cnt = 0;      /* number of terms currently stored */
static size_t cache_cap = 0;       /* allocated capacity of the cache */

/* Return the n-th term of A037744 (a(1) is the first term). */
seq_t a(idx_t n)
{
    if (n == 0)              /* undefined for n < 1; return 0 */
        return (seq_t)0;

    size_t need = (size_t)n;

    /* Grow the cache if necessary */
    if (need > cache_cap) {
        size_t new_cap = need;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)          /* allocation failure – return 0 */
            return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms */
    if (cached_cnt < need) {
        static const seq_t init[5] = { (seq_t)2,
                                       (seq_t)11,
                                       (seq_t)44,
                                       (seq_t)177,
                                       (seq_t)710 };
        for (size_t i = cached_cnt; i < need; ++i) {
            if (i < 5) {
                cache[i] = init[i];
            } else {
                cache[i] = 4 * cache[i-1] + cache[i-4] - 4 * cache[i-5];
            }
        }
        cached_cnt = need;
    }

    return cache[n-1];
}
