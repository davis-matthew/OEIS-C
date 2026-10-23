/*
 * A008735  Molien series for 3-dimensional group [2+,n ] = 2*(n/2).
 * https://oeis.org/A008735
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2*n + 7*(-1)^n + 9)^2 / 192)
 * Range:     u8 n <= 103, u16 n <= 1765, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use 128‑bit intermediate to avoid overflow on the square */
    unsigned __int128 y = (unsigned __int128)2 * n + 9;
    /* add +7 for even n, subtract 7 for odd n */
    if (n & 1u)
        y -= 7;
    else
        y += 7;

    unsigned __int128 sq = y * y;
    unsigned __int128 res = sq / 192;

    return (seq_t)res;
}
