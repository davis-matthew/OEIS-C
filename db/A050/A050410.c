/*
 * A050410  Truncated square pyramid numbers: a(n) = Sum_{k = n..2*n-1} k^2.
 * https://oeis.org/A050410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n*(7*n-1)*(2*n-1)/6
 * Range:     u8 n <= 5, u16 n <= 30, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a 128‑bit intermediate to avoid overflow on 64‑bit seq_t. */
    __int128 nn   = ( __int128 ) n;          /*convert index to 128‑bit integer*/
    __int128 prod = nn * ( 7*nn - 1 ) * ( 2*nn - 1 ); /*n*(7n-1)*(2n-1)*/
    prod /= 6; /* division is exact for all integer n*/
    return (seq_t) prod;
}
