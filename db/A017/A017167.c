/*
 * A017167  a(n) = (9*n)^7.
 * https://oeis.org/A017167
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n)^7
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 62, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t v = (seq_t)n * 9;
    if (v == 0) return 0;
    seq_t r = v;
    for (int i = 0; i < 6; ++i) {
        r *= v;
    }
    return r;
}
