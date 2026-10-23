/*
 * A019583  a(n) = n*(n-1)^4/2.
 * https://oeis.org/A019583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)^4/2
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 97, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(0) = a(1) = 0; avoid underflow when computing n-1 */
    if (n < 2)
        return 0;

    seq_t n_t = (seq_t)n;          /* cast index to seq_t */
    seq_t x   = n_t - 1;           /* n-1 */
    seq_t x2  = x * x;             /* (n-1)^2 */
    seq_t x4  = x2 * x2;           /* (n-1)^4 */

    /* Divide by 2 before the final multiplication to keep values small */
    if (n_t & 1)          /* n odd => (n-1) even, x4 even */
    {
        seq_t half_x4 = x4 / 2;
        return n_t * half_x4;
    }
    else                 /* n even, use n/2 */
    {
        seq_t half_n = n_t / 2;
        return half_n * x4;
    }
}
