/*
 * A172135  Number of ways to place 4 nonattacking knights on an n X n board.
 * https://oeis.org/A172135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^8 - 54*n^6 + 144*n^5 + 1019*n^4 - 5232*n^3 - 2022*n^2
 *            + 51120*n - 77184) / 24 for n >= 6
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 24, u64 n <= 380, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* small values for n = 1..5 */
    static const seq_t small[] = {0, 0, 1, 18, 412, 4436};
    if (n < 6)
        return small[n];

    /* use signed intermediate type to allow temporary negative values */
    __int128 N   = n;
    __int128 N2  = N * N;
    __int128 N3  = N2 * N;
    __int128 N4  = N3 * N;
    __int128 N5  = N4 * N;
    __int128 N6  = N5 * N;
    __int128 N8  = N6 * N2;     /* n^8 */

    __int128 num = N8
                 - (__int128)54  * N6
                 + (__int128)144 * N5
                 + (__int128)1019 * N4
                 - (__int128)5232 * N3
                 - (__int128)2022 * N2
                 + (__int128)51120 * N
                 - (__int128)77184;

    return (seq_t)(num / 24);
}
