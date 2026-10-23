/*
 * A111710  Consider the triangle shown below in which the n-th row contains
 *          the n smallest numbers greater than those in the previous row such
 *          that the arithmetic mean is an integer. Sequence contains the
 *          leading diagonal.
 * https://oeis.org/A111710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 1; for even n, a(n) = (5*n^2 + 6*n)/8; for odd n, a(n) =
 *            (5*n^2 + 4*n - 1)/8
 * Range:     u8 n <= 19, u16 n <= 323, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for 0, safety */
    seq_t N = (seq_t)n;
    if ((n & 1ULL) == 0)            /* even */
        return (5*N*N + 6*N) / 8;
    else                            /* odd */
        return (5*N*N + 4*N - 1) / 8;
}
