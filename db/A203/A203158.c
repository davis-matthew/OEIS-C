/*
 * A203158  v(n+1)/v(n), where v=A203012.
 * https://oeis.org/A203158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=1..n} (i^2 + i*(n+1) + (n+1)^2), i.e.
 *            v(n+1)/v(n) with v = A203012
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 12 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t m = (seq_t)n + 1;
    seq_t m_sq = m * m;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t ii = (seq_t)i;
        result *= ii * ii + ii * m + m_sq;
    }
    return result;
}
