/*
 * A168203  a(n) = 4*n - a(n-1) + 1 with n>1, a(1)=0.
 * https://oeis.org/A168203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n + 3 + 7*(-1)^n)/2
 * Range:     u8 n <= 61, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute 4*n + 3 */
    seq_t val = (seq_t)4 * (seq_t)n + (seq_t)3;

    /* adjust based on parity of n */
    if ((n & (idx_t)1) == 0) {      /* n even: +7 */
        val += (seq_t)7;
    } else {                         /* n odd: -7 */
        val -= (seq_t)7;
    }

    return val / (seq_t)2;
}
