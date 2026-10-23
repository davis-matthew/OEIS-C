/*
 * A144335  Row sums of triangle A144334, binomial transform of [1, 2, 6, 7,
 *          3, 0, 0, 0, ...].
 * https://oeis.org/A144335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n^4 - 2*n^3 + 9*n^2 - 10*n + 24)/24
 * Range:     u8 n <= 6, u16 n <= 27, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use 128‑bit arithmetic to avoid intermediate overflow */
    __int128 N  = (unsigned long long)n;            /* n as 128‑bit */
    __int128 N2 = N * N;
    __int128 N3 = N2 * N;
    __int128 N4 = N3 * N;

    __int128 num = ( __int128)3 * N4
                 - ( __int128)2 * N3
                 + ( __int128)9 * N2
                 - ( __int128)10 * N
                 + ( __int128)24;

    return (seq_t)(num / ( __int128)24);
}
