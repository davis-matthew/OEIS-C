/*
 * A198846  a(n) = 9*6^n - 1.
 * https://oeis.org/A198846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*6^n - 1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;
    for (idx_t i = 0; i < n; ++i) {
        res *= 6;
    }
    res = res * 9 - 1;
    return res;
}
