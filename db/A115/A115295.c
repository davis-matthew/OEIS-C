/*
 * A115295  Partial sums of squares of A115291(n).
 * https://oeis.org/A115295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1, a(1) = 17, a(n) = 64*n - 62 for n >= 2
 * Range:     u8 n <= 4, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (This follows from a(n) = 2*(32*n - 31) for n >= 2.) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)17;
    seq_t nn = (seq_t)n;
    return (seq_t)64 * nn - (seq_t)62;
}
