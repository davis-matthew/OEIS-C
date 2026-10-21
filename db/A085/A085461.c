/*
 * A085461  Number of 5-tuples (v1,v2,v3,v4,v5) of nonnegative integers less
 *          than n such that v1 <= v5, v2 <= v5, v2 <= v4 and v3 <= v4.
 * https://oeis.org/A085461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + 11*binomial(n,2) + 34*binomial(n,3) +
 *            40*binomial(n,4) + 16*binomial(n,5) (equivalently a(n) =
 *            n*(n+1)*(2*n+1)*(2*n*n+2*n+1)/30)
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 125, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binom_small(idx_t n, idx_t k)
{
    if (n < k) return 0;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * ( (seq_t)(n - k + i) );   /* multiply numerator   */
        res = res / (seq_t)i;                 /* divide denominator   */
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* out of defined range */

    seq_t sn = (seq_t)n;          /* n as seq_t */
    seq_t res = sn;               /* term n */

    res += 11 * binom_small(n, 2);
    res += 34 * binom_small(n, 3);
    res += 40 * binom_small(n, 4);
    res += 16 * binom_small(n, 5);

    return res;
}
