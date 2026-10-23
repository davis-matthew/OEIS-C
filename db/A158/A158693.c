/*
 * A158693  a(n) = 66*n^2 - 1.
 * https://oeis.org/A158693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 66*n^2 - 1
 * Range:     u8 n <= 1, u16 n <= 31, u32 n <= 8066, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t val = (seq_t)66 * nn * nn;
    val -= (seq_t)1;
    return val;
}
