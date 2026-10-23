/*
 * A008739  Molien series for 3-dimensional group [2+,n] = 2*(n/2).
 * https://oeis.org/A008739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n^2 + 5*n + 10) / 7)
 * Range:     u8 n <= 39, u16 n <= 674, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a wider type for the intermediate computation to avoid overflow
       for large n, then cast the final result back to seq_t. */
    return (seq_t)(((unsigned __int128)n * n + 5 * (unsigned __int128)n + 10) / 7);
}
