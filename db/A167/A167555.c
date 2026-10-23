/*
 * A167555  The third left hand column of triangle A167552.
 * https://oeis.org/A167555
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (14*n^5 - 115*n^4 + 400*n^3 - 665*n^2 + 486*n - 120)/120
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 131, u64 n <= 1002,
 *            u128 n <= 1002, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Off‑site terms are unspecified, just return 0 for n < 3 */
    if (n < 3)
        return 0;

    /* Use 128‑bit integers for safe intermediate arithmetic. */
    unsigned __int128 N   = (unsigned __int128)n;
    unsigned __int128 N2  = N * N;
    unsigned __int128 N3  = N2 * N;
    unsigned __int128 N4  = N3 * N;
    unsigned __int128 N5  = N4 * N;

    unsigned __int128 num =
        ((unsigned __int128)14 * N5)
      - ((unsigned __int128)115 * N4)
      + ((unsigned __int128)400 * N3)
      - ((unsigned __int128)665 * N2)
      + ((unsigned __int128)486 * N)
      - 120;

    return (seq_t)(num / 120);
}
