/*
 * A016839  a(n) = (4*n+3)^3.
 * https://oeis.org/A016839
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n+3)^3
 * Range:     u8 n <= 0, u16 n <= 9, u32 n <= 200, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n * 4 + 3;
    seq_t res = t * t * t;
    return res;
}
