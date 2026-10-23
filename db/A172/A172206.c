/*
 * A172206  Number of ways to place 7 nonattacking kings on a 7 X n board.
 * https://oeis.org/A172206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (117649*n^7 - 2873997*n^6 + 32197753*n^5 - 215350695*n^4
 *            + 932130286*n^3 - 2618213868*n^2 + 4424623272*n -
 *            3468569760)/720, n>=6. For n<6: a(1)=0, a(2)=0, a(3)=24,
 *            a(4)=926, a(5)=37282
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 14, u64 n <= 276, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first five values directly. */
    if (n == 0)                     /* safety for n<1, not used in OEIS */
        return 0;
    if (n <= 5) {
        static const seq_t tbl[5] = {0, 0, 24, 926, 37282};
        return tbl[n - 1];
    }

    /* For n >= 6 use the closed‑form polynomial.  Intermediate
       arithmetic is performed in signed __int128 to avoid overflow
       for 64‑bit builds while remaining correct on arbitrary‑precision builds. */
    signed __int128 nn   = (signed __int128)n;
    signed __int128 p1   = nn;                 /* n   */
    signed __int128 p2   = p1 * p1;            /* n^2 */
    signed __int128 p3   = p2 * p1;            /* n^3 */
    signed __int128 p4   = p3 * p1;            /* n^4 */
    signed __int128 p5   = p4 * p1;            /* n^5 */
    signed __int128 p6   = p5 * p1;            /* n^6 */
    signed __int128 p7   = p6 * p1;            /* n^7 */

    signed __int128 res =
          117649 * p7
        - 2873997 * p6
        + 32197753 * p5
        - 215350695 * p4
        + 932130286 * p3
        - 2618213868 * p2
        + 4424623272 * p1
        - 3468569760;

    res /= 720;  /* division is exact by construction */

    return (seq_t)res;
}
