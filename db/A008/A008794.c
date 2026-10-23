/*
 * A008794  Squares repeated; a(n) = floor(n/2)^2.
 * https://oeis.org/A008794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = ( floor(n/2) )^2
 * Range:     u8 n <= 31, u16 n <= 511, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to seq_t and compute the floor of n/2. */
    seq_t half = (seq_t)n >> 1;   /* n/2 rounded down */

    /* Return the square of half. */
    return half * half;
}
