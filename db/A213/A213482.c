/*
 * A213482  Number of (w,x,y) with all terms in {0,...,n} and |w-x| + |x-y| >
 *          w+x+y.
 * https://oeis.org/A213482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (23*n^3 + 39*n^2 + n + 9 - (3*n+9)*((n+1) mod 2))/24
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;

    seq_t numerator = 23 * n3 + 39 * n2 + nn + 9;
    /* subtract when (n+1) is odd */
    if (((n + 1) & 1ULL) != 0)
        numerator -= (3 * nn + 9);

    return numerator / 24;
}
