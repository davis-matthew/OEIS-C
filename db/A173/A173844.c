/*
 * A173844  Number of permutations of 1..n with no adjacent pair summing to
 *          n+4.
 * https://oeis.org/A173844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..m} (-2)^j * binomial(m, j) * (n-j)! where m =
 *            max(0, floor((n-3)/2)) (for k = 4 in A173844)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute m = floor((n-3)/2), but not negative */
    idx_t m = 0;
    if (n >= 3)
        m = (n - 3) / 2;

    /* compute factorial n! */
    seq_t fac = 1;
    for (idx_t i = 2; i <= n; ++i)
        fac *= (seq_t)i;

    seq_t sum   = 0;    /* accumulated result */
    seq_t binom = 1;    /* C(m, j), starting with j = 0 */
    seq_t pow2  = 1;    /* 2^j, starting with j = 0 */

    for (idx_t j = 0; j <= m; ++j)
    {
        seq_t term = binom * fac * pow2;

        /* add or subtract depending on parity of j */
        if ((j & 1) == 0)
            sum += term;
        else
            sum -= term;

        /* update for next j, if any */
        if (j == m)
            break;

        /* binom_{j+1} = binom_j * (m - j) / (j + 1) */
        binom = binom * (m - j) / (j + 1);

        /* fac_{j+1} = fac_j / (n - j) */
        fac = fac / (n - j);

        /* pow2_{j+1} = 2 * pow2_j */
        pow2 = pow2 * (seq_t)2;
    }

    return sum;
}
