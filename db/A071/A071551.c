/*
 * A071551  a(n) = (9n)!/n!^9.
 * https://oeis.org/A071551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9n)!/(n!)^9
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 2, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t m = 1; m <= 9; ++m) {
        idx_t bigN = m * n;           /*  m·n  */
        seq_t bin   = 1;
        for (idx_t i = 1; i <= n; ++i) {
            bin = bin * (seq_t)(bigN - n + i) / (seq_t)i;
        }
        result *= bin;
    }
    return result;
}
