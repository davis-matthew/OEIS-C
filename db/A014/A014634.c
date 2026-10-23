/*
 * A014634  a(n) = (2*n+1)*(4*n+1).
 * https://oeis.org/A014634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n+1)*(4*n+1)
 * Range:     u8 n <= 5, u16 n <= 90, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type to preserve precision. */
    seq_t nn = (seq_t)n;
    /* Compute 2*n+1 and 4*n+1 in seq_t arithmetics. */
    seq_t term1 = (seq_t)2 * nn + (seq_t)1;   /* 2*n + 1 */
    seq_t term2 = (seq_t)4 * nn + (seq_t)1;   /* 4*n + 1 */
    /* Result is the product of the two terms. */
    return term1 * term2;
}
