/*
 * A008393  Coordination sequence for A_9 lattice.
 * https://oeis.org/A008393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 + 11*n^2*(221*n^6 + 2730*n^4 + 7917*n^2 + 5260)/2016,
 *            a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 15, u64 n <= 250, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;              /* base case */

    /* use 128‑bit arithmetic for intermediate products */
    unsigned __int128 n128 = n;
    unsigned __int128 n2   = n128 * n128;          /* n^2 */
    unsigned __int128 n4   = n2   * n2;            /* n^4 */
    unsigned __int128 n6   = n4   * n2;            /* n^6 */

    unsigned __int128 poly = 221 * n6
                           + 2730 * n4
                           + 7917 * n2
                           + 5260;             /* polynomial part */

    unsigned __int128 num  = 11 * n2 * poly;        /* numerator */
    unsigned __int128 div  = num / 2016;           /* integer division */

    return 2 + (seq_t)div;                        /* cast back to seq_t */
}
