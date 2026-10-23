/*
 * A152742  13 times the squares: a(n) = 13*n^2.
 * https://oeis.org/A152742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 13*n^2
 * Range:     u8 n <= 4, u16 n <= 71, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute a(n) = 13 * n^2 with all intermediate arithmetic
       in the seq_t type to preserve exactness whenever the result
       fits in seq_t. */
    return (seq_t)n * (seq_t)n * (seq_t)13;
}
