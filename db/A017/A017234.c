/*
 * A017234  a(n) = (9*n + 6)^2.
 * https://oeis.org/A017234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 6)^2
 * Range:     u8 n <= 1, u16 n <= 27, u32 n <= 7281, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t and compute the base */
    seq_t base = (seq_t)9 * n + (seq_t)6;
    /* Return the square of the base */
    return base * base;
}
