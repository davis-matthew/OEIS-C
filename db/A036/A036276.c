/*
 * A036276  a(n) = A001864(n+1)/2.
 * https://oeis.org/A036276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binom(n+1,2) * sum_{j=0}^{n-1} (n+1)^j * (n-1)! / j!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t N  = (seq_t)n;
    seq_t N1 = N + 1;

    /* factorial (n-1)! */
    seq_t fact = 1;
    for (idx_t i = 2; i < n; ++i)
        fact *= (seq_t)i;

    /* term_j = (n-1)! * (n+1)^j / j! */
    seq_t term = fact;
    seq_t sum  = fact;

    for (idx_t j = 0; j + 1 < n; ++j)   /* j = 0 .. n-2 */
    {
        term = term * N1 / ((seq_t)(j + 1));
        sum += term;
    }

    /* binomial coefficient C(n+1,2) = n*(n+1)/2 */
    seq_t binom = N * N1 / 2;

    return binom * sum;
}
