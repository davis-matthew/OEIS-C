/*
 * A203751  Square root of v(2*n+1)/(3*v(2*n)), where v = A203748.
 * https://oeis.org/A203751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2 * product_{k=1}^{n-1} (n^2 + n*k + k^2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t nseq = (seq_t)n;
    seq_t n2   = nseq * nseq;
    seq_t res  = n2;               /* n^2 */
    for (idx_t k = 1; k < n; ++k)
    {
        seq_t kseq = (seq_t)k;
        seq_t term = n2 + nseq * kseq + kseq * kseq; /* n^2 + n*k + k^2 */
        res *= term;
    }
    return res;
}
