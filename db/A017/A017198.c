/*
 * A017198  a(n) = (9*n + 3)^2.
 * https://oeis.org/A017198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 3)^2
 * Range:     u8 n <= 1, u16 n <= 28, u32 n <= 7281, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute (9*n + 3) in seq_t to avoid overflow in idx_t */
    seq_t tmp = (seq_t)n * 9 + 3;
    return tmp * tmp;
}
