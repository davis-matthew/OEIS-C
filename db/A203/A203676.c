/*
 * A203676  v(n+1)/v(n), where v=A203675.
 * https://oeis.org/A203676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=1..n} (i^4 - i^2*(n+1)^2 + (n+1)^4), i.e.
 *            v(n+1)/v(n) with v = A203675
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 9,
 *            big any n
 * Validated: partial, pass, 9 terms through n = 9 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n + 1;
    seq_t m2 = m * m;
    seq_t m4 = m2 * m2;
    seq_t prod = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t i2 = (seq_t)i * (seq_t)i;
        seq_t i4 = i2 * i2;
        prod *= (i4 + m4 - i2 * m2);
    }
    return prod;
}
