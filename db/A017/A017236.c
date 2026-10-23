/*
 * A017236  a(n) = (9*n + 6)^4.
 * https://oeis.org/A017236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 6)^4
 * Range:     u8 none, u16 n <= 1, u32 n <= 27, u64 n <= 7281, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 9*n + 6 in seq_t to avoid overflow */
    seq_t x = (seq_t)9 * n + (seq_t)6;
    /* Square once, then square again to get the fourth power */
    seq_t x2 = x * x;
    return x2 * x2;
}
