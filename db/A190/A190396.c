/*
 * A190396  Number of ways to place 4 nonattacking grasshoppers on a
 *          chessboard of size n x n.
 * https://oeis.org/A190396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^8 - 6*n^6 - 80*n^5 + 431*n^4 - 552*n^3 - 666*n^2 +
 *            2168*n - 1392) / 24, n>2 a(1) = 0, a(2) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 23, u64 n <= 380, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;
    if (n == 2)
        return 1;

    unsigned __int128 x  = (unsigned __int128)n;
    unsigned __int128 n2 = x * x;
    unsigned __int128 n3 = n2 * x;
    unsigned __int128 n4 = n3 * x;
    unsigned __int128 n5 = n4 * x;
    unsigned __int128 n6 = n3 * n3;
    unsigned __int128 n8 = n4 * n4;

    unsigned __int128 val = n8
        - (unsigned __int128)6  * n6
        - (unsigned __int128)80 * n5
        + (unsigned __int128)431 * n4
        - (unsigned __int128)552 * n3
        - (unsigned __int128)666 * n2
        + (unsigned __int128)2168 * x
        - (unsigned __int128)1392;

    val /= 24;
    return (seq_t)val;
}
