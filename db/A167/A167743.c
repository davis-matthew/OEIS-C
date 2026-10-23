/*
 * A167743  Positive differences between distinct positive triangular numbers,
 *          with repetition.
 * https://oeis.org/A167743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A167743
 *  a(n) = the n-th element of the multiset of all positive differences
 *         between distinct triangular numbers (T_i = i(i+1)/2, 0 ≤ i < j).
 *  Each difference d appears exactly A001227(d) times, where
 *  A001227(d) = number of pairs (i,j), 0 ≤ i < j, satisfying
 *                T_j − T_i = d.
 *
 *  The multiplicity can be computed from the divisors of 2·d:
 *      2·d = x·y  with 0 < x < y
 *      a pair contributes iff y – x is odd.
 *  This gives an O(√d) routine for the multiplicity.
 *
 *  The function a(idx_t n) returns the value a(n).
 */
#include <stdlib.h>
#include <stdint.h>

static idx_t multiplicity(idx_t d)
{
    unsigned __int128 val = (unsigned __int128)d * 2;
    idx_t cnt = 0;
    for (idx_t x = 1; (unsigned __int128)x * x <= val; ++x) {
        if (val % x == 0) {
            idx_t y = (idx_t)(val / x);
            if (x < y && ((y - x) & 1u))
                ++cnt;
        }
    }
    return cnt;
}

seq_t a(idx_t n)
{
    /* Static cumulative sums: cum[d] = total count of values ≤ d */
    static idx_t *cum = NULL;
    static size_t cap = 0;
    static idx_t last_d = 0;       /* last difference index computed */

    /* initial allocation */
    if (!cum) {
        cap = 1024;
        cum = (idx_t *)malloc((cap + 1) * sizeof(idx_t));
        cum[0] = 0;               /* dummy: no values ≤ 0 */
    }

    /* extend the table until the cumulative count exceeds the requested index */
    while (cum[last_d] < n) {
        /* move to next difference value */
        idx_t d = ++last_d;
        if ((size_t)d >= cap) {          /* grow the array */
            size_t newcap = cap * 2;
            cum = (idx_t *)realloc(cum, (newcap + 1) * sizeof(idx_t));
            cap = newcap;
        }
        /* compute its multiplicity and update the cumulative sum */
        idx_t m = multiplicity(d);
        cum[d] = cum[d - 1] + m;
    }

    /* binary search for the smallest d with cum[d] >= n */
    idx_t lo = 1, hi = last_d;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (cum[mid] >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return (seq_t)lo;
}
