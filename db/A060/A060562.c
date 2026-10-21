/*
 * A060562  Number of ways to color vertices of a 10-gon using <= n colors,
 *          allowing rotations and reflections.
 * https://oeis.org/A060562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^10 + 5*n^6 + 6*n^5 + 4*n^2 + 4*n)/20
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 12, u64 n <= 113, u128 n <= 500,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic for intermediate values to avoid overflow
       when seq_t is limited to 8/16/32/64 bits.  The final result
       fits into seq_t for all values that will be tested. */
    unsigned __int128 sn   = (unsigned __int128)n;    /* n in 128‑bit */
    unsigned __int128 n2   = sn * sn;                 /* n^2 */
    unsigned __int128 n4   = n2 * n2;                 /* n^4 */
    unsigned __int128 n5   = n4 * sn;                 /* n^5 */
    unsigned __int128 n6   = n4 * n2;                 /* n^6 */
    unsigned __int128 n10  = n5 * n5;                 /* n^10 */

    unsigned __int128 num  = n10
                           + (unsigned __int128)5 * n6
                           + (unsigned __int128)6 * n5
                           + (unsigned __int128)4 * n2
                           + (unsigned __int128)4 * sn;

    return (seq_t)(num / (unsigned __int128)20);
}
