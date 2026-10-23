/*
 * A181973  Prime-generating polynomial: a(n) = 16*n^2 - 300*n + 1447.
 * https://oeis.org/A181973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*n^2 - 300*n + 1447
 * Range:     u8 none, u16 n <= 73, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return ((seq_t)16 * nn * nn) - ((seq_t)300 * nn) + ((seq_t)1447);
}
