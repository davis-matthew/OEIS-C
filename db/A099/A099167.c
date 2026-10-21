/*
 * A099167  G.f.: (1+x^2)/((1-3x)(1-x-x^2)).
 * https://oeis.org/A099167
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) - 3*a(n-3), with initial values
 *            a(0)=1, a(1)=4, a(2)=15. This recurrence comes from a(n) = 2⋅3^n
 *            – Fib(n+2)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)15;

    seq_t a0 = (seq_t)1;   /* a(n-3) */
    seq_t a1 = (seq_t)4;   /* a(n-2) */
    seq_t a2 = (seq_t)15;  /* a(n-1) */
    seq_t an = 0;          /* a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        an = (seq_t)4 * a2 - (seq_t)2 * a1 - (seq_t)3 * a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }

    return a2;
}
