/*
 * A051867  15-gonal (or pentadecagonal) numbers: n*(13n-11)/2.
 * https://oeis.org/A051867
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(13*n-11)/2
 * Range:     u8 n <= 6, u16 n <= 100, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = n;            /* cast index to seq_t */
    return nn * (nn * (seq_t)13 - (seq_t)11) / (seq_t)2;
}
