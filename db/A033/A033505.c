/*
 * A033505  Expansion of 1/(1 - 3*x - x^2 + x^3).
 * https://oeis.org/A033505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + a(n-2) - a(n-3) with initial values a(0)=1,
 *            a(1)=3, a(2)=10
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)10;

    seq_t a0 = 1;      /* a(n-3)   */
    seq_t a1 = 3;      /* a(n-2)   */
    seq_t a2 = 10;     /* a(n-1)   */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t ai = 3 * a2 + a1 - a0;  /* a(n)     */
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }
    return a2;
}
