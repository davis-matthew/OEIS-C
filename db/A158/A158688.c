/*
 * A158688  a(n) = 1089*n^2 + 33.
 * https://oeis.org/A158688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1089*n^2 + 33
 * Range:     u8 n <= 0, u16 n <= 7, u32 n <= 1985, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t val = 1089;
    val *= nn;
    val *= nn;
    val += 33;
    return val;
}
