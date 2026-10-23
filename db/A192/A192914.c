/*
 * A192914  Constant term in the reduction by (x^2 -> x + 1) of the polynomial
 *          C(n)*x^n, where C=A000285.
 * https://oeis.org/A192914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - a(n-3) * Initial values: a(0)=1,
 *            a(1)=0, a(2)=5
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)5;

    seq_t a0 = (seq_t)1;      /* a(n-3) */
    seq_t a1 = (seq_t)0;      /* a(n-2) */
    seq_t a2 = (seq_t)5;      /* a(n-1) */
    seq_t next;

    for (idx_t i = 3; i <= n; ++i) {
        /* next = 2*a2 + 2*a1 - a0; implemented using additions */
        next = a2; next += a2;      /* 2*a2 */
        next += a1; next += a1;      /* +2*a1 */
        next -= a0;                  /* - a0 */

        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
