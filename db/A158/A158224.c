/*
 * A158224  a(n) = 196*n^2 - 2*n.
 * https://oeis.org/A158224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 196*n^2 - 2*n
 * Range:     u8 n <= 1, u16 n <= 18, u32 n <= 4681, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return sn * sn * 196 - sn * 2;
}
