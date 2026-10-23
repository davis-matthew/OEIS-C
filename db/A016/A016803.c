/*
 * A016803  a(n) = (4*n)^3.
 * https://oeis.org/A016803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n)^3
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 406, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t mult = (seq_t)64;   /* 4^3 = 64 */
    return mult * nn * nn * nn;
}
