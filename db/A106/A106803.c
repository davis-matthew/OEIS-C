/*
 * A106803  Expansion of x*(1-x)/(1-2*x-x^2+x^3).
 * https://oeis.org/A106803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - a(n-3), a(0)=0, a(1)=a(2)=1
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 29, u64 n <= 56, u128 n <= 111,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1 || n == 2) return (seq_t)1;

    /* initialise the last three terms: a0 = a(n-3), a1 = a(n-2), a2 = a(n-1) */
    seq_t a0 = (seq_t)0;  /* a(0)  */
    seq_t a1 = (seq_t)1;  /* a(1)  */
    seq_t a2 = (seq_t)1;  /* a(2)  */

    for (idx_t i = 3; i <= n; ++i) {
        /* compute a3 = 2*a2 + a1 - a0 */
        seq_t a3 = a2;
        a3 += a2;           /* 2*a2 */
        a3 += a1;           /* + a1 */
        a3 -= a0;           /* - a0 */

        /* shift the window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }
    return a2;
}
