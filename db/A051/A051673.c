/*
 * A051673  Cubic star numbers: a(n) = n^3 + 4*Sum_{i=0..n-1} i^2.
 * https://oeis.org/A051673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = n*(n*(7*n-6)+2)/3
 * Range:     u8 n <= 5, u16 n <= 30, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit intermediate to avoid overflow of the intermediate
       calculations when seq_t might be only 64 bits. */
    __int128 nn = ( __int128 ) n;
    __int128 inner = nn * ( ( __int128 )7 * nn - ( __int128 )6 ) + ( __int128 )2;
    __int128 res   = nn * inner / ( __int128 )3;
    return (seq_t)res;
}
