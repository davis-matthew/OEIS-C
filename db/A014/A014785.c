/*
 * A014785  a(n) = Sum_{0<=k<=n} ceiling(k^2/n).
 * https://oeis.org/A014785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} ceil(k^2 / n)
 * Range:     u8 n <= 26, u16 n <= 441, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* result is 0 for n = 0, though offset is 1 */

    seq_t sum = 0;
    seq_t N   = (seq_t)n;        /* cast once for division */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t K    = (seq_t)k;
        seq_t term = (K * K + N - 1) / N; /* ceil(k^2/n) */
        sum += term;
    }
    return sum;
}
