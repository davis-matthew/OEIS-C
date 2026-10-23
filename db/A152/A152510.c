/*
 * A152510  1/60 of the number of permutations of 5 indistinguishable copies
 *          of 1..n with exactly 3 local maxima.
 * https://oeis.org/A152510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n)=382*a(n-1)-38020*a(n-2)+1394280*a(n-3)-17690400*a(n-4)
 *            +92123136*a(n-5)-170698752*a(n-6) for n>6
 * Initial values: a(1)=0, a(2)=2, a(3)=1066, a(4)=328314,
 *                a(5)=87554515, a(6)=22414176982
 * (All arithmetic performed modulo the range of seq_t)
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                          /* undefined, but safe */

    /* static memoisation structure */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;          /* allocated array length */
    static idx_t  max_computed = 0;        /* highest n stored in cache */

    /* ensure array covers indices 0..n */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 16;  /* small over‑alloc */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;               /* allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* return cached value if already computed */
    if (max_computed >= n)
        return cache[n];

    /* initial known terms (indices 1..6) */
    static const seq_t init[7] = {
        (seq_t)0,
        (seq_t)0,         /* a(1) */
        (seq_t)2,         /* a(2) */
        (seq_t)1066,      /* a(3) */
        (seq_t)328314,    /* a(4) */
        (seq_t)87554515,  /* a(5) */
        (seq_t)22414176982ULL  /* a(6) */
    };

    /* compute values up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i <= 6) {
            cache[i] = init[i];
        } else {
            /* compute via the recurrence using seq_t only */
            seq_t t = (seq_t)0;
            t += (seq_t)382      * cache[i-1];
            t -= (seq_t)38020    * cache[i-2];
            t += (seq_t)1394280  * cache[i-3];
            t -= (seq_t)17690400 * cache[i-4];
            t += (seq_t)92123136 * cache[i-5];
            t -= (seq_t)170698752* cache[i-6];
            cache[i] = t;
        }
    }

    max_computed = n;
    return cache[n];
}
