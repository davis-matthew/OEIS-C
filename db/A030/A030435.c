/*
 * A030435  Expansion of g.f. (1 + x - 2*x^2 - x^3)/(1/2 - 2*x^2 + x^4).
 * https://oeis.org/A030435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-2) - 2*a(n-4) for n ≥ 4 initial values: a(0)=2,
 *            a(1)=2, a(2)=4, a(3)=6
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 36, u64 n <= 72, u128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* handle small n directly */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)6;

    /* recurrence window: a0=a(n-4), a1=a(n-3), a2=a(n-2), a3=a(n-1) */
    seq_t a0 = (seq_t)2;   /* a(n-4) */
    seq_t a1 = (seq_t)2;   /* a(n-3) */
    seq_t a2 = (seq_t)4;   /* a(n-2) */
    seq_t a3 = (seq_t)6;   /* a(n-1) */
    seq_t an = 0;          /* current term */

    for (idx_t i = 4; i <= n; ++i) {
        an = (seq_t)4 * a2 - (seq_t)2 * a0;
        /* shift window for next n */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
