/*
 * A203767  v(n+1)/v(n), where v=A203766.
 * https://oeis.org/A203767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=1..n} (ceil(i/2)^2 + ceil((n+1)/2)^2), i.e.
 *            v(n+1)/v(n) with v = A203766
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 14 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    seq_t m = ((seq_t)n + 2) / 2;   /* ceil((n+1)/2) */
    seq_t m2 = m * m;
    seq_t result = 1;
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t c = ((seq_t)i + 1) / 2;
        result *= (c * c + m2);
    }
    return result;
}
