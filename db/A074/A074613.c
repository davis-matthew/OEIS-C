/*
 * A074613  a(n) = 4^n + 7^n.
 * https://oeis.org/A074613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4^n + 7^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow4 = 1;
    seq_t pow7 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow4 *= (seq_t)4;
        pow7 *= (seq_t)7;
    }
    return pow4 + pow7;
}
