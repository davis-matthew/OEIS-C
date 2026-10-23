/*
 * A187021  Coefficient of x^n in (1 + (n+1)*x + n*x^2)^n.
 * https://oeis.org/A187021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)^2 * n^k
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum    = 1;                    /* term for k = 0 */
    seq_t bin    = 1;                    /* current binomial coefficient C(n,k) */
    seq_t n_pow  = 1;                    /* n^k */
    seq_t nseq   = (seq_t)n;             /* cast once for multiplication */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* Update binomial coefficient C(n,k) */
        bin = bin * (seq_t)(n - k + 1) / (seq_t)k;

        /* Update n^k */
        n_pow = n_pow * nseq;

        /* term = bin^2 * n^k */
        seq_t term = bin * bin;
        term = term * n_pow;

        /* accumulate */
        sum += term;
    }
    return sum;
}
