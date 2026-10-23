/*
 * A190315  Central coefficients of the Riordan matrix
 *          ((1-x-x^2)/(1-2x-x^2),(x-x^2-x^3)/(1-2x-x^2)) (A190215).
 * https://oeis.org/A190315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=0..n} binomial(n+i,n) * Sum_{j=0..n-i}
 *            binomial(i+j-1,j) * binomial(j,n-i-j)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses incremental computation of the inner binomials
   to run in O(n^2) time and O(1) additional memory. */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0u)
        return 1;

    seq_t total  = 0;
    seq_t factor = 1;           /* binomial(n+0,n) */

    for (idx_t i = 0; i <= n; ++i) {
        idx_t limit = n - i;    /* maximum j for this i */

        if (limit == 0) {       /* only j = 0 term, binomial(0,0)=1 */
            total += factor;
        } else {
            idx_t j_start = (limit + 1) / 2;   /* ceil(limit/2) */

            /* Compute c1 = binomial(i+j-1,j) up to j_start */
            seq_t c1 = 1;                  /* j = 0 */
            for (idx_t j = 1; j <= j_start; ++j)
                c1 = c1 * (i + j - 1) / j;

            /* c2 for the first j in the inner sum */
            seq_t c2 = binom(j_start, limit - j_start);

            for (idx_t j = j_start; ; ++j) {
                seq_t term = c1 * c2;
                total += factor * term;

                if (j == limit)
                    break;

                /* Prepare next values of c1 and c2 */

                idx_t next_j = j + 1;
                seq_t c1_next = c1 * (i + next_j - 1) / next_j;

                /* Update c2 using the ratio formula */
                idx_t m      = limit - j;               /* current m = n-i-j */
                idx_t denom1 = 2 * j + i - n + 2;
                idx_t denom2 = 2 * j + i - n + 1;
                seq_t denom    = (seq_t)denom1 * (seq_t)denom2;
                seq_t numerator = c2 * (next_j) * m;
                seq_t c2_next = numerator / denom;

                c1 = c1_next;
                c2 = c2_next;
            }
        }

        /* Update factor for next i (binomial(n+i+1,n)) */
        if (i < n)
            factor = factor * (n + i + 1) / (i + 1);
    }

    return total;
}
