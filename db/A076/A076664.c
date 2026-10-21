/*
 * A076664  a(n) = Sum_{k=1..n} antisigma(k), where antisigma(i) = sum of the
 *          nondivisors of i that are between 1 and i.
 * https://oeis.org/A076664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)/6 - Σ_{i=1..n} i * floor(n/i)
 * Range:     u8 n <= 12, u16 n <= 73, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for the OEIS sequence A076664. */
seq_t a(idx_t n)
{
    /* cast n to seq_t once for further multiplication */
    seq_t N = (seq_t)n;
    /* total sum of k(k+1)/2 up to n = n(n+1)(n+2)/6 */
    seq_t total = N * (N + 1) * (N + 2) / 6;

    /* sum_{k=1..n} sigma(k) = Σ_{d=1..n} d * floor(n/d) */
    seq_t sum_sigma = 0;
    for (idx_t d = 1; d <= n; ++d)
    {
        /* floor division by d */
        idx_t cnt = n / d;
        sum_sigma += (seq_t)d * (seq_t)cnt;
    }

    /* a(n) = total - sum_sigma */
    return total - sum_sigma;
}
