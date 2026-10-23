/*
 * A008734  Molien series for 3-dimensional group [2+,n ] = 2*(n/2).
 * https://oeis.org/A008734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2*n+7+5*(-1)^n)^2/128)
 * Range:     u8 n <= 85, u16 n <= 1443, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute 2*n in seq_t to avoid overflow in idx_t */
    seq_t two_n = (seq_t)n * 2;
    seq_t x;

    if ((n & 1) == 0)  /* n is even */
        x = two_n + 12;   /* 2*n + 7 + 5 = 2*n + 12 */
    else               /* n is odd  */
        x = two_n + 2;    /* 2*n + 7 - 5 = 2*n + 2 */

    /* Floor division by 128 */
    return (x * x) / 128;
}
