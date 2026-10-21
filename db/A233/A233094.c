/*
 * A233094  Number of n X 4 0..3 arrays with no element x(i,j) adjacent to
 *          value 3-x(i,j) horizontally, diagonally or antidiagonally, top
 *          left element zero, and 1 appearing before 2 in row major order.
 * https://oeis.org/A233094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 31*a(n-1) - 35*a(n-2) + 5*a(n-3) with a(1)=14, a(2)=386,
 *            a(3)=11486 (For n≥4 this linear recurrence holds.)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Assume seq_t and idx_t are defined by the build. */
seq_t a(idx_t n)
{
    /* a(0) is not defined for this offset; return 0 if called. */
    if (n == 0) return (seq_t)0;

    /* static cache for memoisation */
    static seq_t *cache = NULL;   /* cache[0] unused, cache[i] = a(i) */
    static size_t  cap   = 0;     /* allocated capacity                  */
    static size_t  sz    = 0;     /* highest index stored (>=3 after init) */

    /* initialise cache on first call */
    if (!cache) {
        cap = 16;                                 /* start capacity */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;             /* allocation failed */

        /* dummy entry for index 0 */
        cache[0] = (seq_t)0;
        /* seed the first three terms */
        cache[1] = (seq_t)14;
        cache[2] = (seq_t)386;
        cache[3] = (seq_t)11486;
        sz = 3;
    }

    /* grow cache if needed */
    if ((idx_t)sz < n) {
        size_t need = (size_t)n + 1;          /* +1 because indices start at 0 */
        if (need > cap) {
            while (cap < need) cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;       /* allocation failed */
            cache = tmp;
        }
        /* compute up to requested n using the recurrence */
        for (idx_t i = sz + 1; i <= n; ++i) {
            seq_t val = (seq_t)31 * cache[(size_t)(i - 1)];
            val = val - (seq_t)35 * cache[(size_t)(i - 2)];
            val = val + (seq_t)5  * cache[(size_t)(i - 3)];
            cache[(size_t)i] = val;
        }
        sz = (size_t)n;
    }

    return cache[(size_t)n];
}
