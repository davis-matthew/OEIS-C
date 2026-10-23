/*
 * A116220  If X_1,...,X_n is a partition of a 4n-set X into 4-blocks then
 *          a(n) is equal to the number of permutations f of X such that
 *          f(X_i) <> X_i, (i=1,...,n).
 * https://oeis.org/A116220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..n} (-24)^j * binomial(n,j) * (4*n-4*j)!
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle offset; for n==0 return 0, otherwise compute normally */
    if (n == 0) return 0;

    /* pos accumulates terms with even j, neg with odd j */
    seq_t pos = 0, neg = 0;

    /* binom starts as C(n,0)=1 */
    seq_t binom = 1;
    /* fact starts as (4*n)! */
    seq_t fact = 1;
    /* compute (4*n)!  */
    const idx_t four_n = 4 * n;
    for (idx_t i = 1; i <= four_n; ++i)
        fact *= (seq_t)i;

    /* power of 24, starting as 24^0 = 1 */
    seq_t pow24 = 1;

    for (idx_t j = 0; j <= n; ++j) {
        /* current term = binom * fact * pow24 */
        seq_t term = binom * fact * pow24;

        /* add or subtract depending on parity of j */
        if ((j & 1) == 0)
            pos += term;
        else
            neg += term;

        /* prepare for next j unless we are at the end */
        if (j == n)
            break;

        /* update factorial: divide by (4n-4j)*(4n-4j-1)*(4n-4j-2)*(4n-4j-3) */
        for (int t = 0; t < 4; ++t)
            fact /= (seq_t)(four_n - 4*j - t);

        /* update binomial: C(n,j+1) = C(n,j)*(n-j)/(j+1) */
        binom = binom * (seq_t)(n - j) / (seq_t)(j + 1);

        /* update power of 24 */
        pow24 *= 24;
    }

    return pos - neg;
}
