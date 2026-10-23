/*
 * A164093  a(n) = 9*4^n + 2.
 * https://oeis.org/A164093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*4^n + 2
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 1;
    for (idx_t i = 0; i < n; ++i) {
        val *= 4;
    }
    val = val * 9 + 2;
    return val;
}
