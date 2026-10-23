/*
 * A192916  Constant term in the reduction by (x^2 -> x+1) of the polynomial
 *          C(n)*x^n, where C=A022095.
 * https://oeis.org/A192916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - a(n-3) Initial values: a(0)=1,
 *            a(1)=0, a(2)=6 Recurrence uses only integer arithmetic on seq_t
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Handle the initial cases directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)6;

    /* Use only the three most recent terms to compute the next one. */
    seq_t a0 = (seq_t)1;  /* a(n-3)  */
    seq_t a1 = (seq_t)0;  /* a(n-2)  */
    seq_t a2 = (seq_t)6;  /* a(n-1)  */
    seq_t a3;             /* current term */

    for (idx_t i = 3; i <= n; ++i) {
        a3 = (seq_t)2 * a2 + (seq_t)2 * a1 - a0; /* recurrence */
        a0 = a1;    /* shift window */
        a1 = a2;
        a2 = a3;
    }
    return a2; /* a(n) */
}
