/*
 * A192753  Coefficient of x in the reduction by x^2->x+1 of the polynomial
 *          p(n,x) defined below in Comments.
 * https://oeis.org/A192753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Let A_0 = 0, A_1 = 1. For n >= 2, A_n = A_{n-1} + A_{n-2} +
 *            4*(n-1) + 3. a(n) = A_n. This gives the coefficient of x after
 *            reducing p(n,x) by x^2 -> x+1
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* Just to be safe; types are supplied by the build */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t a_prevprev = 0; /* A_0 */
    seq_t a_prev     = 1; /* A_1 */

    for (idx_t k = 2; k <= n; ++k) {
        seq_t const term = (seq_t)4 * (seq_t)(k - 1) + (seq_t)3;
        seq_t const cur  = a_prev + a_prevprev + term;
        a_prevprev = a_prev;
        a_prev     = cur;
    }
    return a_prev;
}
