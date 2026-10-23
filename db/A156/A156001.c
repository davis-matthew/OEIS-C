/*
 * A156001  Number of cycles of length 4 in the queen graph associated with an
 *          n X n chessboard.
 * https://oeis.org/A156001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(21*n^3+526*n^2-1709*n+996)/60
 *            -(3*n^2-12*n+4)*floor(n/2)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use signed 128‑bit arithmetic to avoid overflow of intermediate results */
    __int128 nn = ( __int128 )n;          /* promote idx_t to signed __int128 */
    if (nn < 2) return (seq_t)0;          /* a(0) = a(1) = 0 */

    __int128 floor_n2 = nn >> 1;          /* floor(n/2) */

    __int128 n2  = nn * nn;
    __int128 n3  = n2 * nn;
    __int128 poly = ( (__int128)21 * n3 )
                  + ( (__int128)526 * n2 )
                  - ( (__int128)1709 * nn )
                  + ( (__int128)996 );

    __int128 prod  = nn * (nn - 1) * poly;
    __int128 part1 = prod / 60;

    __int128 temp2 = ( (__int128)3 * n2 )
                   - ( (__int128)12 * nn )
                   + ( (__int128)4 );
    __int128 part2 = temp2 * floor_n2;

    __int128 result = part1 - part2;
    return (seq_t)result;
}
