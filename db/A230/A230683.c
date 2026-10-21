/*
 * A230683  Number of n X 4 0..6 arrays x(i,j) with each element horizontally
 *          or antidiagonally next to at least one element with value
 *          (x(i,j)+1) mod 7, and upper left element zero.
 * https://oeis.org/A230683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 22, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence for OEIS A230683:
 * a(1)=0, a(2)=0, a(3)=0, a(4)=2, a(5)=32, a(6)=432, a(7)=5182,
 * a(8)=58298, a(9)=631576
 * For n ≥ 10:
 *   a(n) = 17*a(n-1) - 64*a(n-2) - 66*a(n-3) + 20*a(n-4) + 14*a(n-5)
 *          + 671*a(n-6) + 1404*a(n-7) - 76*a(n-8) + a(n-9)
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static storage for memoisation */
    static seq_t *cache   = NULL;      /* cache[0] unused, cache[i] = a(i) */
    static idx_t  size    = 0;         /* allocated size (including 0)   */
    static idx_t  max_n   = 0;         /* largest index for which a(i) known */

    /* minimal required size so that indices 0..max(n,9) are addressable */
    idx_t needed = (n > 9 ? n : 9) + 1;
    if (cache == NULL || size < needed) {
        /* reallocate */
        seq_t *tmp = (seq_t*)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) return (seq_t)0;     /* allocation failure – unlikely */
        /* initialise new entries to zero */
        for (idx_t i = size; i < needed; ++i) tmp[i] = (seq_t)0;
        cache = tmp;
        size  = needed;
    }

    /* initialise the first nine terms (they are constant for all runs) */
    cache[1] = (seq_t)0;                /* a(1)  */
    cache[2] = (seq_t)0;                /* a(2)  */
    cache[3] = (seq_t)0;                /* a(3)  */
    cache[4] = (seq_t)2;                /* a(4)  */
    cache[5] = (seq_t)32;               /* a(5)  */
    cache[6] = (seq_t)432;              /* a(6)  */
    cache[7] = (seq_t)5182;             /* a(7)  */
    cache[8] = (seq_t)58298;            /* a(8)  */
    cache[9] = (seq_t)631576;           /* a(9)  */

    if (n <= 9) return cache[n];

    /* compute any missing terms up to n */
    idx_t start = max_n + 1;            /* first index we need to compute */
    if (start < 10) start = 10;         /* recurrence valid from 10 */
    for (idx_t i = start; i <= n; ++i) {
        seq_t val = 0;
        val += cache[i - 1] * (seq_t)17;
        val += cache[i - 4] * (seq_t)20;
        val += cache[i - 5] * (seq_t)14;
        val += cache[i - 6] * (seq_t)671;
        val += cache[i - 7] * (seq_t)1404;
        val += cache[i - 9];

        val -= cache[i - 2] * (seq_t)64;
        val -= cache[i - 3] * (seq_t)66;
        val -= cache[i - 8] * (seq_t)76;

        cache[i] = val;
    }
    if (n > max_n) max_n = n;

    return cache[n];
}
