/*
 * A017419  a(n) = (11*n + 2)^7.
 * https://oeis.org/A017419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 2)^7
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 51, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n * (seq_t)11 + (seq_t)2;
    seq_t r = x;
    for (idx_t i = 1; i < 7; ++i) {
        r *= x;
    }
    return r;
}
