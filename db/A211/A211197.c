/*
 * A211197  Table T(n,k) = 2*n + ((-1)^n)*(1/2 - (k-1) mod 2) - 1/2; n, k > 0,
 *          read by antidiagonals.
 * https://oeis.org/A211197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 8255, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   _max_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(n) = T(i,j) where (i,j) are the unique coordinates of n in the
 *   antidiagonal ordering with i>=1, j>=1.  For odd i, rows alternate
 *   between 2*i-1 (for odd j) and 2*i (for even j).  For even i, the
 *   alternation is reversed.
 *
 *   i = n - t*(t+1)/2
 *   j = (t*t + 3*t + 4)/2 - n
 *   t = floor((-1 + sqrt(8*n-7))/2)   -- the largest integer with
 *   t*(t+1)/2 < n <= (t+1)*(t+2)/2
 *
 *   All operations are performed with integer arithmetic only.
 */

#include <stdint.h>   /* for uint64_t (idx_t) */
#include "../utils/_max_t.h"

seq_t a(idx_t n)
{
    /* n must be >=1 according to OEIS offset */
    idx_t t = _max_t(n);

    /* triangular number of t */
    idx_t tri = (idx_t)(((__uint128_t)t * (t + 1)) / 2);
    idx_t i = n - tri;                      /* 1 <= i <= t+1 */

    /* compute j from the diagonal length */
    __uint128_t tmp = (__uint128_t)t * t + (__uint128_t)3 * t + 4;
    idx_t j = (idx_t)(tmp / 2 - n);          /* 1 <= j <= t+1 */

    /* alternate between 2*i-1 and 2*i depending on parity of i and j */
    if ((i & 1) == 1) {                     /* odd row */
        if ((j & 1) == 1)   /* odd column */
            return (seq_t)2 * (seq_t)i - 1;
        else                /* even column */
            return (seq_t)2 * (seq_t)i;
    } else { /* even row */
        if ((j & 1) == 1)   /* odd column */
            return (seq_t)2 * (seq_t)i;
        else                /* even column */
            return (seq_t)2 * (seq_t)i - 1;
    }
}
