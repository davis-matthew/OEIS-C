/*
 * A051868  16-gonal (or hexadecagonal) numbers: a(n) = n*(7*n-6).
 * https://oeis.org/A051868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(7*n-6)
 * Range:     u8 n <= 6, u16 n <= 97, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return nn * (7 * nn - 6);
}
