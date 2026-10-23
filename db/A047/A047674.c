/*
 * A047674  Row 5 of square array defined in A047671.
 * https://oeis.org/A047674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/5)*(2*n^5 - 5*n^4 + 20*n^3 - 25*n^2 + 23*n - 10)
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    __int128 nn = n;
    __int128 v =  (__int128)2 * nn * nn * nn * nn * nn
                - (__int128)5 * nn * nn * nn * nn
                + (__int128)20 * nn * nn * nn
                - (__int128)25 * nn * nn
                + (__int128)23 * nn
                - (__int128)10;
    v /= 5;
    return (seq_t)v;
}
