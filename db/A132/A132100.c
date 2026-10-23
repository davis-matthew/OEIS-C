/*
 * A132100  Number of distinct Tsuro tiles which are square and have n points
 *          per side.
 * https://oeis.org/A132100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((4n-1)!! + Σ_{j=0..n} 2^j * binom(2n,2j) * (2j-1)!! + 2
 *            * Σ_{j=0..floor(n/2)} 4^j * binom(n,2j) * (2j-1)!! ) / 4 where
 *            (2k-1)!! = 1·3·5···(2k-1), (−1)!! is taken as 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n;

    /* term1 = (4m-1)!! */
    seq_t term1 = 1;
    for (idx_t i = 1; i <= 2 * m; ++i) {
        term1 *= (seq_t)(2 * i - 1);
    }

    seq_t term2 = 0;
    seq_t term3 = 0;

    /* double factorial (2j-1)!! for current j */
    seq_t double_fact = 1;

    /* powers of 2 and 4 for current j */
    seq_t power2 = 1;
    seq_t power4 = 1;

    /* binom(2m, 2j) and its current even k */
    seq_t binom2m = 1;        /* binom(2m,0) */
    idx_t k2m = 0;            /* current k in binom(2m,k) */

    /* binom(m, 2j) and its current even k */
    seq_t binomm = 1;        /* binom(m,0) */
    idx_t km = 0;            /* current k in binom(m,k) */
    idx_t max_j = m / 2;     /* largest j for third sum */

    for (idx_t j = 0; j <= m; ++j) {
        /* term2 contribution */
        term2 += power2 * binom2m * double_fact;

        /* term3 contribution (only when j <= max_j) */
        if (j <= max_j) {
            term3 += power4 * binomm * double_fact;
        }

        /* Update for next j */
        if (j < m) {
            /* update double factorial */
            double_fact *= (seq_t)(2 * (j + 1) - 1);

            /* update power2 */
            power2 *= 2;

            /* update binom(2m,2j) to binom(2m,2(j+1)) */
            seq_t num = (seq_t)(2 * m - k2m);
            num *= (seq_t)(2 * m - k2m - 1);
            seq_t denom = (seq_t)(k2m + 1);
            denom *= (seq_t)(k2m + 2);
            binom2m = (binom2m * num) / denom;
            k2m += 2;
        }

        if (j < max_j) {
            /* update power4 */
            power4 *= 4;

            /* update binom(m,2j) to binom(m,2(j+1)) */
            seq_t num = (seq_t)(m - km);
            num *= (seq_t)(m - km - 1);
            seq_t denom = (seq_t)(km + 1);
            denom *= (seq_t)(km + 2);
            binomm = (binomm * num) / denom;
            km += 2;
        }
    }

    /* multiply third sum by 2 as per formula */
    term3 *= 2;

    /* final result */
    seq_t sum = term1 + term2 + term3;
    return sum / 4;
}
