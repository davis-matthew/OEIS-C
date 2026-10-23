/*
 * A192460  Constant term of the reduction by x^2->x+1 of the polynomial
 *          p(n,x) defined below in Comments.
 * https://oeis.org/A192460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = constant term of the reduction by x^2→x+1 of p(n−1,x)
 *            where p(k,x) = ∏_{i=0}^{k} (i + (i+1)x). It satisfies a(1)=0,
 *            a(2)=2, a(3)=13, … as in A192460
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* behaviour below the offset may be undefined */

    idx_t k;
    seq_t a_val = 0;   /* constant term a_0 for p(0,x)=x */
    seq_t b_val = 1;   /* coefficient of x in p(0,x)=x */

    for (k = 1; k < n; ++k) {
        seq_t a_next = (seq_t)k * a_val + (seq_t)(k + 1) * b_val;
        seq_t b_next = (seq_t)(k + 1) * a_val + (seq_t)(2 * k + 1) * b_val;
        a_val = a_next;
        b_val = b_next;
    }
    return a_val;   /* a(n) = a_{n-1} */
}
