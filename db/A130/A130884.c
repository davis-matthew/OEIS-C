/*
 * A130884  a(n) = 3n^3 + 2n^2 + n + 1.
 * https://oeis.org/A130884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n^3 + 2*n^2 + n + 1
 * Range:     u8 n <= 4, u16 n <= 27, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = n;
    seq_t res = ((seq_t)3 * nn * nn * nn) + ((seq_t)2 * nn * nn) + nn + (seq_t)1;
    return res;
}
