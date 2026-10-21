/*
 * A052863  Expansion of e.g.f. log(-1/(-1+x))*exp(x) - log(-1/(-1+x)).
 * https://oeis.org/A052863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0}^{n-2} k! * binomial(n, k+1)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;               /* a(0)=0, a(1)=0 */

    seq_t result = 0;
    seq_t fact = 1;            /* 0! */
    seq_t binom = (seq_t)n;    /* C(n,1) */

    result += fact * binom;    /* k = 0 */

    for (idx_t k = 1; k <= n - 2; ++k)
    {
        fact = fact * (seq_t)k;                                 /* k! */
        binom = binom * (seq_t)(n - k) / (seq_t)(k + 1);        /* C(n,k+1) */
        result += fact * binom;
    }

    return result;
}
