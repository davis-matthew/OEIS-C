/*
 * A017280  a(n) = (10*n)^12.
 * https://oeis.org/A017280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n)^12
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 0, u64 n <= 4, u128 n <= 162,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ten_n = (seq_t)10 * (seq_t)n;
    seq_t res = (seq_t)1;
    for (idx_t i = 0; i < 12; ++i)
        res *= ten_n;
    return res;
}
