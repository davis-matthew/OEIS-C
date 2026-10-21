/*
 * A081626  a(n) = 2*6^n-4^n.
 * https://oeis.org/A081626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*6^n - 4^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow6 = 1;
    seq_t pow4 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow6 = pow6 * 6;
        pow4 = pow4 * 4;
    }
    seq_t result = pow6 * 2;
    result = result - pow4;
    return result;
}
