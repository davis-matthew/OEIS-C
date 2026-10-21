/*
 * A086114  Number of 4 X n (0,1) matrices such that each row and each column
 *          is nondecreasing or nonincreasing.
 * https://oeis.org/A086114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2/3)*n*(n^3+6*n^2+11*n-6)
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn  = (seq_t)n;          /* n as seq_t */
    seq_t n2  = nn * nn;           /* n^2 */
    seq_t n3  = n2 * nn;           /* n^3 */
    seq_t t   = n3 + 6 * n2 + 11 * nn - 6; /* n^3+6n^2+11n-6 */
    seq_t prod= 2 * nn * t;        /* 2*n*t */
    return prod / 3;               /* division is exact for all n>=1 */
}
