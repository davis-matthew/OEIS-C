/*
 * A017165  a(n) = (9*n)^5.
 * https://oeis.org/A017165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n)^5
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 9, u64 n <= 792, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n * (seq_t)9;   /* x = 9*n */
    seq_t r = x;                    /* r = x^1 */
    r *= x;                         /* r = x^2 */
    r *= x;                         /* r = x^3 */
    r *= x;                         /* r = x^4 */
    r *= x;                         /* r = x^5 */
    return r;
}
