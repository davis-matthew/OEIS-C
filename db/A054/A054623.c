/*
 * A054623  Number of ways to color vertices of a 9-gon using <= n colors,
 *          allowing only rotations.
 * https://oeis.org/A054623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^8+2*n^2+6)/9
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 15, u64 n <= 176, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use an unsigned __int128 temporary to avoid overflow on 64‑bit builds */
    unsigned __int128 nn   = (unsigned __int128)n;
    unsigned __int128 n2   = nn * nn;          /* n^2 */
    unsigned __int128 n4   = n2 * n2;          /* n^4 */
    unsigned __int128 n8   = n4 * n4;          /* n^8 */
    unsigned __int128 term = n8 + 2 * n2 + 6;   /* n^8 + 2*n^2 + 6 */
    unsigned __int128 res  = nn * term;        /* n*(n^8 + 2*n^2 + 6) */
    res /= 9;                                 /* division by 9 (exact) */
    return (seq_t)res;
}
