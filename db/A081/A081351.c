/*
 * A081351  First row in square maze array of natural numbers A081349.
 * https://oeis.org/A081351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n+2) - (n+1)*(-1)^n
 * Range:     u8 n <= 14, u16 n <= 254, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n + 1;          /* m = n + 1 */
    if ((n & 1) == 0)                /* n even: a(n) = (n+1)^2 */
        return m * m;
    else                             /* n odd : a(n) = (n+1)*(n+3) */
        return m * ((seq_t)n + 3);
}
