/*
 * A017431  a(n) = (11*n + 3)^7.
 * https://oeis.org/A017431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 3)^7
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 51, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = 11 * (seq_t)n + 3;
    seq_t res = 1;
    for (idx_t i = 0; i < 7; ++i)
        res *= x;
    return res;
}
