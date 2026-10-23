/*
 * A016767  a(n) = (3*n)^3.
 * https://oeis.org/A016767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n)^3
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 250, u64 n <= 250,
 *            u128 n <= 250, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t res = nn * nn;
    res *= nn;
    res *= (seq_t)27;
    return res;
}
