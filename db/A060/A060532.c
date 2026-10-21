/*
 * A060532  Number of ways to color vertices of a heptagon using <= n colors,
 *          allowing rotations and reflections.
 * https://oeis.org/A060532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^7 + 7*n^4 + 6*n) / 14
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 34, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use unsigned __int128 for intermediate arithmetic so that
     * the calculation works correctly even when seq_t is 64‑bit
     * and the intermediate overflows that range.  The final
     * result fits into seq_t because the formula is exact.     */
    unsigned __int128 nn = (unsigned __int128) n;          /* n                    */
    unsigned __int128 n2 = nn * nn;                      /* n^2                   */
    unsigned __int128 n4 = n2 * n2;                      /* n^4                   */
    unsigned __int128 n3 = n2 * nn;                      /* n^3                   */
    unsigned __int128 n7 = n4 * n3;                      /* n^7                   */
    unsigned __int128 num = n7 + 7 * n4 + 6 * nn;         /* numerator             */
    /* division by 14 is exact for all n, so we can use division.   */
    unsigned __int128 res = num / 14;
    return (seq_t)res;
}
