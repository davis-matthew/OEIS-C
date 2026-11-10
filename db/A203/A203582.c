/*
 * A203582  v(n+1)/v(n), where v=A203581.
 * https://oeis.org/A203582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A203581(n+1)/A203581(n) = Product_{x=2..n+1} (x^2 +
 *            x*(n+2) + (n+2)^2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 12 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t y = (seq_t)n + 2;
    seq_t y_sq = y * y;
    seq_t result = 1;
    for (idx_t i = 2; i <= n + 1; ++i) {
        seq_t x = (seq_t)i;
        result *= x * x + x * y + y_sq;
    }
    return result;
}
