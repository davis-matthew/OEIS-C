/*
 * A017309  a(n) = (10*n + 3)^5.
 * https://oeis.org/A017309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 3)^5
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 8, u64 n <= 712, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute (10*n + 3)^5 using only integer arithmetic. */
    seq_t t = (seq_t)n * 10 + 3;
    seq_t r = t * t * t * t * t;
    return r;
}
