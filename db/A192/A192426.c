/*
 * A192426  Constant term of the reduction by x^2 -> x+1 of the polynomial
 *          p(n,x) defined below in Comments.
 * https://oeis.org/A192426
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+5*a(n-2)-2*a(n-3)-4*a(n-4), a(0) = 2, a(1) = 0,
 *            a(2) = 5, a(3) = 1
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 51, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Initial values for n = 0..3 */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)1;

    /* Working variables for the recurrence */
    seq_t a0 = (seq_t)2;   /* a(n-4) */
    seq_t a1 = (seq_t)0;   /* a(n-3) */
    seq_t a2 = (seq_t)5;   /* a(n-2) */
    seq_t a3 = (seq_t)1;   /* a(n-1) */
    seq_t an = a3;         /* placeholder for a(n) */

    /* Compute a(k) for k = 4 .. n */
    for (idx_t k = 4; k <= n; ++k) {
        an = a3 + (seq_t)5 * a2 - (seq_t)2 * a1 - (seq_t)4 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
