/*
 * A017157  a(n) = (8*n + 7)^9.
 * https://oeis.org/A017157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n + 7)^9
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 16, u128 n <= 2388,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)8 * (seq_t)n + 7;
    seq_t r = 1;
    for (idx_t i = 0; i < 9; ++i)
        r *= x;
    return r;
}
