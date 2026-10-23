/*
 * A186030  a(n) = n*(13*n-3)/2.
 * https://oeis.org/A186030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(13*n-3)/2
 * Range:     u8 n <= 4, u16 n <= 94, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t, if needed */

seq_t a(idx_t n)
{
    seq_t sn = n;                                   /* cast index to seq_t */
    seq_t temp = (seq_t)13 * sn - (seq_t)3;         /* compute 13*n - 3   */
    seq_t prod = sn * temp;                         /* compute n * (13*n-3) */
    return prod / (seq_t)2;                         /* done, final division by 2 */
}
