/*
 * A173843  Number of permutations of 1..n with no adjacent pair summing to
 *          n+3.
 * https://oeis.org/A173843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..m} (-2)^j * binomial(m,j) * (n-j)!, where m =
 *            max(0, floor((n-2)/2)). This is the OEIS formula for A173843.
 *            The implementation uses the provided binomial() helper and
 *            performs the calculation with arbitrary‑precision arithmetic (or
 *            native unsigned types). The summands with even j...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* m = floor((n-2)/2) with a lower bound of 0 */
    idx_t m = (n >= 2) ? ((n - 2) / 2) : 0;

    /* compute factorial(n) */
    seq_t fact = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) fact *= (seq_t)i;

    /* iterative computation of sum */
    seq_t sum_pos = (seq_t)0;
    seq_t sum_neg = (seq_t)0;
    seq_t pow2   = (seq_t)1;
    seq_t fact_term = fact;   /* currently (n-j)! with j=0 */

    for (idx_t j = 0; j <= m; ++j) {
        seq_t bin = binomial(m, j);          /* binomial(m,j)   */
        seq_t term = fact_term * bin;        /* (n-j)!*C(m,j)   */
        term *= pow2;                       /* multiply by 2^j   */

        if ((j & 1ULL) == 0)
            sum_pos += term;
        else
            sum_neg += term;

        /* prepare for next j */
        pow2 <<= 1;                          /* *2  */
        if (j < m)                           /* update factorial(n-(j+1)) */
            fact_term /= (seq_t)(n - j);
    }

    /* final result: sum_pos - sum_neg  (sum_pos >= sum_neg) */
    return sum_pos - sum_neg;
}
