/*
 * A017400  a(n) = (11*n)^12.
 * https://oeis.org/A017400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n)^12
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 0, u64 n <= 3, u128 n <= 147,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)11 * (seq_t)n;
    seq_t res = 1;
    for (idx_t i = 0; i < 12; ++i)
        res *= base;
    return res;
}
