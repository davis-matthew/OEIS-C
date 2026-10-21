/*
 * A052933  Expansion of (1-x^2)/(1 - x - 3*x^2 + 2*x^4).
 * https://oeis.org/A052933
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - 2*a(n-4); * Initial values: a(0)=1,
 *            a(1)=1, a(2)=3, a(3)=6 * This implements the linear recurrence
 *            derived from the generating function * (1-x^2)/(1 - x - 3*x^2 +
 *            2*x^4). The routine handles any * sequence element that fits in
 *            seq_t; for arbitrary‑precision builds * seq...
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 57, u128 n <= 114,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)6;

    /* Use the recurrence a(n)=a(n-1)+3*a(n-2)-2*a(n-4). */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)1;   /* a(1) */
    seq_t a2 = (seq_t)3;   /* a(2) */
    seq_t a3 = (seq_t)6;   /* a(3) */
    seq_t an;

    for (idx_t i = 4; i <= n; ++i) {
        /* Compute a(i) using the relation and rotate the window. */
        an = a3 + (seq_t)3 * a2 - (seq_t)2 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}
