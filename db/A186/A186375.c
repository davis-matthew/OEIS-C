/*
 * A186375  a(n) equals the sum of the squares of the expansion coefficients
 *          for (x + y + 2*z)^n.
 * https://oeis.org/A186375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} C(n,k)^2 * C(2*k,k) * 4^(n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    /* Base case */
    if (n == 0) return 1;

    /* c1  = C(n, k)   : start with k = 0 */
    seq_t c1 = 1;
    /* c2  = C(2*k, k) : start with k = 0  (C(0,0) = 1) */
    seq_t c2 = 1;
    /* p4  = 4^(n-k)  : start with k = 0 */
    seq_t p4 = pow4(n);

    seq_t sum = 0;

    for (idx_t k = 0; k <= n; ++k)
    {
        /* term = c1^2 * c2 * p4 */
        seq_t term = c1 * c1;
        term = term * c2;
        term = term * p4;
        sum += term;

        if (k == n) break;          /* last iteration done */

        /* Prepare values for k+1 */

        /* C(n, k+1) = C(n, k) * (n-k) / (k+1) */
        seq_t denom = (seq_t)(k + 1);
        c1 = c1 * ((seq_t)(n - k)) / denom;

        /* C(2*(k+1), k+1) = C(2k, k) * (2*(k+1))*(2*(k+1)-1) / ((k+1)*(k+1)) */
        seq_t num = (seq_t)(2 * (k + 1)) * (seq_t)(2 * (k + 1) - 1);
        c2 = c2 * num / (denom * denom);

        /* 4^(n-(k+1)) = 4^(n-k) / 4 */
        p4 = p4 / 4;
    }

    return sum;
}
