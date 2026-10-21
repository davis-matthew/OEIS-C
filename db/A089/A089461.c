/*
 * A089461  Hyperbinomial transform of A088957. Also the row sums of triangle
 *          A089460, which lists the coefficients for the second hyperbinomial
 *          transform.
 * https://oeis.org/A089461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 2*(n-k+2)^(n-k-1)*C(n, k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 27,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    seq_t sum   = 0;
    seq_t comb  = 1;            /* C(n,0) */

    for (idx_t k = 0; k <= n; ++k) {
        idx_t t = n - k;        /* t = n - k */

        seq_t term;
        if (t == 0) {           /* 2*(2)^(-1) = 1 */
            term = 1;           /* comb is C(n, n) = 1 here */
        } else if (t == 1) {    /* 2*(3)^0 = 2 */
            term = 2 * comb;
        } else {                /* general case */
            seq_t base  = (seq_t)(t + 2);
            seq_t power = pow_seq(base, (int)(t - 1));  /* (t+2)^(t-1) */
            term = power * 2 * comb;
        }

        sum += term;

        /* Update comb to C(n, k+1) for next iteration */
        if (k < n) {
            comb = comb * (n - k) / (k + 1);
        }
    }

    return sum;
}
