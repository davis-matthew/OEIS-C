/*
 * A192954  Constant term of the reduction by x^2 -> x+1 of the polynomial
 *          p(n,x) defined at Comments.
 * https://oeis.org/A192954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) - a(n-3) + a(n-4) a(0) = 1, a(1) = 1,
 *            a(2) = 5, a(3) = 11
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for idx_t/seq_t types and macros only */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)11;

    /* Iterate using the linear recurrence */
    seq_t a0 = (seq_t)1;   /* a(n-4) */
    seq_t a1 = (seq_t)1;   /* a(n-3) */
    seq_t a2 = (seq_t)5;   /* a(n-2) */
    seq_t a3 = (seq_t)11;  /* a(n-1) */
    seq_t an = (seq_t)0;

    for (idx_t i = 4; i <= n; ++i) {
        an = (seq_t)3 * a3 - (seq_t)2 * a2 - a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
