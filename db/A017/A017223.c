/*
 * A017223  a(n) = (9*n+5)^3.
 * https://oeis.org/A017223
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n+5)^3
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 180, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to seq_t first to avoid overflow of idx_t  */
    seq_t v = ((seq_t)n) * 9 + 5;
    /* Compute the cube */
    return v * v * v;
}
