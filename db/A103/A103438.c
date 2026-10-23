/*
 * A103438  Square array T(m,n) read by antidiagonals: Sum_{k=1..n} k^m.
 * https://oeis.org/A103438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Sum_{k=1}^{n'} k^m where m is the antidiagonal index of n
 *            such that m(m+1)/2 ≤ n < (m+1)(m+2)/2 and n' = n - m(m+1)/2 (0 ≤
 *            n' ≤ m). The sum is over k from 1 to n'. This reads the square
 *            array T(m,n) = Σ_{k=1..n} k^m along antidiagonals
 * Range:     u8 n <= 38, u16 n <= 81, u32 n <= 194, u64 n <= 471,
 *            u128 n <= 1186, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the diagonal index m for this position. */
    idx_t m = 0;
    while ((m + 1) * (m + 2) / 2 <= n) {
        ++m;
    }
    /* Previous diagonal gives the offset of the start of this diagonal. */
    idx_t prev = m * (m + 1) / 2;
    idx_t offset = n - prev;          /* 0 ≤ offset ≤ m */
    idx_t exponent = m - offset;      /* m' in the formula above */
    idx_t bound = offset;             /* n' in the formula above */

    /* Compute the sum Σ_{k=1}^{bound} k^exponent. */
    seq_t sum = 0;
    for (idx_t k = 1; k <= bound; ++k) {
        seq_t pow = 1;
        for (idx_t e = 0; e < exponent; ++e) {
            pow *= k;
        }
        sum += pow;
    }
    return sum;
}
