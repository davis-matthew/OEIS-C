/*
 * A183986  T(n,k) = 1/4 the number of (n+1) X (k+1) binary arrays with all 2
 *          X 2 subblock sums the same.
 * https://oeis.org/A183986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 136, u32 n <= 528, u64 n <= 683,
 *            u128 n <= 683, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A183986
 *
 *  a(n) enumerates the values of the table T(i,j), 1 ≤ i,j,
 *  where T(i,j) = -2
 *                + 2^(i-1) + 2^(j-1)
 *                + 2^⌊(i-1)/2⌋ + 2^⌊i/2⌋
 *                + 2^⌊(j-1)/2⌋ + 2^⌊j/2⌋
 *
 *  The terms are taken in the order of increasing sum i+j,
 *  and within each diagonal by increasing i (hence decreasing j).
 *
 *  For example:
 *      n = 1  -> (i,j) = (1,1) -> T = 4
 *      n = 2  -> (i,j) = (1,2) -> T = 6
 *      n = 3  -> (i,j) = (2,1) -> T = 6
 *      n = 4  -> (i,j) = (1,3) -> T = 9
 *      …
 *
 *  The implementation uses only integer arithmetic and the provided
 *  utilities.  It runs in O(log n) time per call and is correct
 *  for all indices that fit in the arithmetic type of the build.
 */

#include "../utils/pow2.h"

static seq_t table_term(idx_t i, idx_t j)
{
    seq_t res = 0;
    res -= (seq_t)2;                     /* initialise with -2 */

    res += pow2(i - 1);
    res += pow2(j - 1);
    res += pow2((i - 1) / 2);
    res += pow2(i / 2);
    res += pow2((j - 1) / 2);
    res += pow2(j / 2);

    return res;
}

seq_t a(idx_t n)
{
    /* Find the diagonal s = i + j for which the n‑th term lies.
     * The number of terms with diagonal sum < s is
     *   N(s-1) = (s-2)*(s-1)/2
     * We need the smallest s with (s-1)*s/2 >= n. */
    idx_t lo = 2, hi = 2;

    while ( ((unsigned __int128)(hi - 1) * hi) / 2 < n )
        hi *= 2;

    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 cnt = ((unsigned __int128)(mid - 1) * mid) / 2;
        if (cnt >= n)
            hi = mid;
        else
            lo = mid + 1;
    }

    idx_t s  = lo;                      /* diagonal sum i + j */
    idx_t prev = ((s - 2) * (s - 1)) / 2;
    idx_t pos  = n - prev;              /* position within diagonal (1 … s-1) */

    idx_t i = pos;                      /* i = pos on the diagonal */
    idx_t j = s - pos;                  /* j = s - i */

    return table_term(i, j);
}
