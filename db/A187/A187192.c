/*
 * A187192  Number of 5-turn rook's tours on an n X n board summed over all
 *          starting positions.
 * https://oeis.org/A187192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 16*n^6 - 98*n^5 + 228*n^4 - 238*n^3 + 92*n^2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a signed 128‑bit intermediate so negative terms are handled
       correctly. This is safe for all n where the result fits in a
       128‑bit integer (n ≤ 10^4).  For larger n the 64‑bit build
       simply discards the value as it falls outside the tested range. */
    __int128 nn = (__int128)n;
    __int128 n2 = nn * nn;           /* n^2          */
    __int128 n3 = n2 * nn;           /* n^3          */
    __int128 n5 = n3 * n2;           /* n^5          */

    __int128 term1 = n5 * ( (__int128)16 * nn - 98 );   /* 16 n^6 - 98 n^5   */
    __int128 term2 = n3 * ( (__int128)228 * nn - 238 );  /* 228 n^4 - 238 n^3 */
    __int128 term3 = (__int128)92 * n2;                   /* 92 n^2            */

    __int128 result = term1 + term2 + term3;

    return (seq_t)result;
}
