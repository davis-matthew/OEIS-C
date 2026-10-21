/*
 * A224509  Expansion of
 *          (1-x)*(1-2*x)*(1-3*x)/(1-9*x+28*x^2-35*x^3+15*x^4-x^5).
 * https://oeis.org/A224509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A224509
   Recurrence:  a(n) = 9*a(n-1) - 28*a(n-2) + 35*a(n-3) - 15*a(n-4) + a(n-5)
   Initial values: a(0)=1, a(1)=3, a(2)=10, a(3)=35, a(4)=125
   Computed iteratively with memoization. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of computed terms */
    static size_t cache_cap = 0;  /* allocated capacity */

    /* allocate initial memory if needed */
    if (cache == NULL) {
        cache_cap = 64; /* arbitrary initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failed */
    }

    /* grow cache if necessary */
    while (cache_len <= n) {
        /* ensure there is room for the next element */
        if (cache_len + 1 > cache_cap) {
            cache_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (tmp == NULL) return (seq_t)0; /* allocation failed */
            cache = tmp;
        }

        /* compute the next term */
        if (cache_len == 0) {
            cache[0] = (seq_t)1;
        } else if (cache_len == 1) {
            cache[1] = (seq_t)3;
        } else if (cache_len == 2) {
            cache[2] = (seq_t)10;
        } else if (cache_len == 3) {
            cache[3] = (seq_t)35;
        } else if (cache_len == 4) {
            cache[4] = (seq_t)125;
        } else {
            /* recurrence for n >= 5 */
            seq_t t1 = (seq_t)9 * cache[cache_len - 1];
            seq_t t2 = (seq_t)28 * cache[cache_len - 2];
            seq_t t3 = (seq_t)35 * cache[cache_len - 3];
            seq_t t4 = (seq_t)15 * cache[cache_len - 4];
            cache[cache_len] = t1 - t2 + t3 - t4 + cache[cache_len - 5];
        }

        cache_len++;
    }

    return cache[n];
}
