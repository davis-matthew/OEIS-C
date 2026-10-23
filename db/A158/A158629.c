/*
 * A158629  a(n) = 484*n^2 + 22.
 * https://oeis.org/A158629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 484*n^2 + 22
 * Range:     u8 n <= 0, u16 n <= 11, u32 n <= 2978, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return nn * nn * (seq_t)484 + (seq_t)22;
}
