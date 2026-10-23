/*
 * A116829  Number of permutations of length n which avoid the patterns 1234,
 *          1324, 3421.
 * https://oeis.org/A116829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n^6 - 15*n^5 - 125*n^4 + 255*n^3 + 3358*n^2 - 11400*n
 *            + 9360)/720 for n ≥ 4; a(1) = 1, a(2) = 2, a(3) = 6
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)6;

    __int128 N = (__int128)n;
    __int128 n2 = N * N;
    __int128 n3 = n2 * N;
    __int128 n4 = n3 * N;
    __int128 n5 = n4 * N;
    __int128 n6 = n5 * N;

    __int128 val =  ( (__int128)7   * n6 )
                  - ( (__int128)15  * n5 )
                  - ( (__int128)125 * n4 )
                  + ( (__int128)255 * n3 )
                  + ( (__int128)3358 * n2 )
                  - ( (__int128)11400 * N )
                  + ( (__int128)9360 );

    val /= (__int128)720;

    return (seq_t)val;
}
