/*
 * A175151  a(n) = Sum_{i=1..n} ((i+1)^i - 1)/i.
 * https://oeis.org/A175151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{i=1..n} ((i+1)^i-1)/i
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t term   = 0;      /* sum_{j=1..i} C(i,j) * i^{j-1} */
        seq_t binom  = 1;      /* C(i,0) = 1 */
        seq_t pow_   = 1;      /* i^{j-1} , start with j=1 -> i^0 = 1 */

        for (idx_t j = 1; j <= i; ++j)
        {
            /* Update binomial coefficient C(i,j) */
            binom = (binom * (seq_t)(i - j + 1)) / (seq_t)j;

            /* Add current term */
            term += binom * pow_;

            /* Prepare i^{j} for next iteration */
            pow_ *= (seq_t)i;
        }
        sum += term;
    }
    return sum;
}
