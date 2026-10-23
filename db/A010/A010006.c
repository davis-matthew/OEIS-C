/*
 * A010006  Coordination sequence for C_3 lattice: a(n) = 16*n^2 + 2 (n>0),
 *          a(0)=1.
 * https://oeis.org/A010006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*n^2 + 2 (a(0)=1)
 * Range:     u8 n <= 3, u16 n <= 63, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    seq_t nn = (seq_t)n;
    return 16 * nn * nn + 2;
}
