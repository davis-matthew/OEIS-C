/*
 * A061166  Polynomial extrapolation of 2, 3, 5, 7, 11, 13, 17.
 * https://oeis.org/A061166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            (23*n^6-537*n^5+4925*n^4-22515*n^3+53732*n^2-61548*n+27360)/720
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Use 128‑bit arithmetic for intermediate values to avoid overflow on 64‑bit builds.
       The final division by 720 is exact for integer n >= 1. */
    unsigned __int128 nn = (unsigned __int128)n;          /* n as 128‑bit integer */
    unsigned __int128 n2 = nn * nn;                      /* n^2 */
    unsigned __int128 n3 = n2 * nn;                      /* n^3 */
    unsigned __int128 n4 = n3 * nn;                      /* n^4 */
    unsigned __int128 n5 = n4 * nn;                      /* n^5 */
    unsigned __int128 n6 = n5 * nn;                      /* n^6 */

    unsigned __int128 num =  (unsigned __int128)23  * n6
                           - (unsigned __int128)537 * n5
                           + (unsigned __int128)4925 * n4
                           - (unsigned __int128)22515 * n3
                           + (unsigned __int128)53732 * n2
                           - (unsigned __int128)61548 * nn
                           + (unsigned __int128)27360;

    /* Division by 720; result fits in seq_t for the terms that are expected to be checked. */
    return (seq_t)(num / 720);
}
