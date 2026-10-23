/*
 * A152759  3 times 9-gonal (or nonagonal) numbers: a(n) = 3*n*(7*n-5)/2.
 * https://oeis.org/A152759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n*(7*n-5)/2
 * Range:     u8 n <= 3, u16 n <= 68, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;
    seq_t temp = (seq_t)(7 * nn - 5);
    seq_t prod = 3 * nn * temp;
    return prod / 2;
}
