/*
 * A157627  a(n) = 8000n - 6040.
 * https://oeis.org/A157627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8000*n - 6040
 * Range:     u8 none, u16 n <= 8, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return nn * (seq_t)8000u - (seq_t)6040u;
}
