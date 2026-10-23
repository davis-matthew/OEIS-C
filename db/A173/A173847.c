/*
 * A173847  Number of permutations of 1..n with no adjacent pair summing to
 *          n+7.
 * https://oeis.org/A173847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..max(0,floor((n-6)/2))} (-2)^j *
 *            binom(max(0,floor((n-6)/2)), j) * (n-j)!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* Compute m = max(0, floor((n - 6) / 2)) */
    idx_t m = (n >= 6) ? (n - 6) / 2 : 0;

    /* Initial factorial (n)! */
    seq_t term = factorial(n);

    /* Coefficient components: c = C(m, j), pow2 = 2^j, coef = c * pow2 */
    seq_t c = 1;       /* C(m,0) */
    seq_t pow2 = 1;    /* 2^0 */
    seq_t coef = 1;    /* C(m,0) * 2^0 */

    /* Accumulators for even (positive) and odd (negative) contributions */
    seq_t pos = term;  /* j = 0 is even, so add to pos */
    seq_t neg = 0;

    /* Iterate over remaining j values */
    for (idx_t j = 1; j <= m; ++j) {
        /* Update term to (n-j)! */
        term = term / (seq_t)(n - j + 1);

        /* Update binomial coefficient C(m, j) */
        c = c * (seq_t)(m - j + 1) / j;

        /* Update power of two */
        pow2 = pow2 * 2;

        /* New coefficient */
        coef = c * pow2;

        /* Add contribution with appropriate sign */
        if ((j & 1) == 0) {
            pos += term * coef;
        } else {
            neg += term * coef;
        }
    }

    /* Result is the difference between even and odd parts */
    return pos - neg;
}
