/*
 * A173848  Number of permutations of 1..n with no adjacent pair summing to n
 *          + 8.
 * https://oeis.org/A173848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..m} (-2)^j * C(m,j) * (n-j)! , where m = max(0,
 *            floor((n-7)/2)) (k = 8)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Compute m = max(0, floor((n-7)/2)) */
    idx_t m = (n >= 7) ? ((n - 7) / 2) : 0;

    /* Compute n! (fact_n) */
    seq_t fact_n = 1;
    for (idx_t i = 2; i <= n; ++i) {
        fact_n *= (seq_t)i;
    }

    /* Variables for the summation */
    seq_t sum = 0;
    seq_t binom = 1;          /* C(m,0) = 1 */
    seq_t pow2 = 1;           /* 2^0 = 1 */
    seq_t fact_current = fact_n;  /* (n-j)! when j=0 */

    for (idx_t j = 0; j <= m; ++j) {
        /* term = (-2)^j * C(m,j) * (n-j)! */
        seq_t term = binom * fact_current * pow2;
        if (j & 1)          /* odd j: subtract */
            sum -= term;
        else                /* even j: add */
            sum += term;

        /* Advance to next j, if any */
        if (j < m) {
            /* Update 2^j */
            pow2 <<= 1;                     /* multiply by 2 */

            /* Update binomial coefficient C(m, j+1) */
            binom = binom * (seq_t)(m - j) / (seq_t)(j + 1);

            /* Update factorial (n - (j+1))! from (n - j)! */
            fact_current /= (seq_t)(n - j);
        }
    }

    return sum;
}
