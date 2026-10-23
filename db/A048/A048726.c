/*
 * A048726  a(n) = Xmult(n,6), or 2*A048724(n).
 * https://oeis.org/A048726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * (n ^ (n << 1)) = (n ^ (n << 1)) << 1
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast to seq_t to avoid issues with idx_t overflow when shifting. */
    seq_t x = (seq_t)n;
    seq_t y = x << 1;         /* n * 2, as seq_t */
    seq_t z = x ^ y;          /* n XOR (n << 1) */
    return z << 1;            /* multiply by 2 to get the final value */
}
