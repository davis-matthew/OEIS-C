/*
 * A218690  Sum_{k=0..n} C(n,k)^4*C(n+k,k)^2.
 * https://oeis.org/A218690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0..n} C(n,k)^4 * C(n+k,k)^2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;         /* accumulating sum */
    seq_t c_nk = 1;        /* C(n,0) */
    seq_t c_mk = 1;        /* C(n+0,0) = 1 */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* term = C(n,k)^4 * C(n+k,k)^2 */
        seq_t t = c_nk * c_nk;   /* C(n,k)^2   */
        t = t * t;              /* (C(n,k)^2)^2 = C(n,k)^4 */
        seq_t u = c_mk * c_mk;   /* C(n+k,k)^2 */
        t = t * u;              /* multiply */

        sum = sum + t;

        if (k == n)            /* finished all terms */
            break;

        /* update binomials for next k */
        seq_t num1 = (seq_t)(n - k);
        seq_t den1 = (seq_t)(k + 1);
        c_nk = c_nk * num1 / den1;

        seq_t num2 = (seq_t)(n + k + 1);
        seq_t den2 = (seq_t)(k + 1);
        c_mk = c_mk * num2 / den2;
    }
    return sum;
}
