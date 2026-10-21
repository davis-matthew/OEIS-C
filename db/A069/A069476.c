/*
 * A069476  First differences of A069475, successive differences of (n+1)^6 -
 *          n^6.
 * https://oeis.org/A069476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 720*n + 1800
 * Range:     u8 none, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t first, then compute */
    return ((seq_t)n * 720) + 1800;
}
