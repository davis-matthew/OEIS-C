/*
 * A074579  a(n) = 6^n + 8^n + 9^n.
 * https://oeis.org/A074579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6^n + 8^n + 9^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow6 = (seq_t)1;
    seq_t pow8 = (seq_t)1;
    seq_t pow9 = (seq_t)1;

    seq_t const6 = (seq_t)6;
    seq_t const8 = (seq_t)8;
    seq_t const9 = (seq_t)9;

    idx_t i;
    for (i = 0; i < n; ++i) {
        pow6 *= const6;
        pow8 *= const8;
        pow9 *= const9;
    }

    return pow6 + pow8 + pow9;
}
