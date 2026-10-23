/*
 * A163935  Third right hand column of triangle A163934.
 * https://oeis.org/A163935
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-2)*(n-1)*n*(n+1)*(n+2)*n*(3n+5) / 144
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 41, u64 n <= 982, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3u)
        return 0;

    unsigned __int128 prod = 1;
    prod *= (unsigned __int128)(n - 2);
    prod *= (unsigned __int128)(n - 1);
    prod *= (unsigned __int128)n;
    prod *= (unsigned __int128)(n + 1);
    prod *= (unsigned __int128)(n + 2);
    prod *= (unsigned __int128)n;
    prod *= (unsigned __int128)(3 * (unsigned __int128)n + 5);
    prod /= 144;
    return (seq_t)prod;
}
