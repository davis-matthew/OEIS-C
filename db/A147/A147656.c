/*
 * A147656  The arithmetic mean of the n-th and (n+1)-st cubes, rounded down.
 * https://oeis.org/A147656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2n^2+3n+3)/2
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned __int128 nn = n;
    unsigned __int128 t  = (unsigned __int128)2 * nn * nn
                         + (unsigned __int128)3 * nn
                         + 3;
    unsigned __int128 prod = nn * t;
    unsigned __int128 res  = prod / 2;
    return (seq_t)res;
}
