/*
 * A203764  v(n+1)/v(n), where v=A203763.
 * https://oeis.org/A203764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = v(n+1)/v(n) = prod_{i=1..n} (x_i^2 - x_i*y + y^2), x_i =
 *            ceil(i/2), y = ceil((n+1)/2), where v = A203763
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 15 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    seq_t y = (seq_t)((n + 2) >> 1);   /* ceil((n+1)/2) */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t x = (seq_t)((i + 1) >> 1);   /* ceil(i/2) */
        result *= x * x - x * y + y * y;
    }
    return result;
}
