/*
 * A127896  Expansion of 1/(1 + 2*x + 3*x^2 + x^3).
 * https://oeis.org/A127896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -2*a(n-1) -3*a(n-2) - a(n-3); a(0)=1, a(1)=-2, a(2)=1
 * Range:     i8 n <= 12, i16 n <= 24, i32 n <= 51, i64 n <= 103,
 *            i128 n <= 208, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-2;
    if (n == 2) return (seq_t)1;
    seq_t a0 = (seq_t)1;   /* a(n-3)   */
    seq_t a1 = (seq_t)-2;  /* a(n-2)   */
    seq_t a2 = (seq_t)1;   /* a(n-1)   */
    seq_t an;
    for (idx_t i = 3; i <= n; ++i) {
        an = - (seq_t)2 * a2 - (seq_t)3 * a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return an;
}
