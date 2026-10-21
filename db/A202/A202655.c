/*
 * A202655  Number of ways to place 4 nonattacking semi-queens on an n X n
 *          board.
 * https://oeis.org/A202655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 25, u64 n <= 382,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A202655: Number of ways to place 4 non‑attacking semi‑queens on an
 * n X n board.
 *
 * Formula: a(n) = n^8/24 - 2*n^7/3 + 41*n^6/9 - 257*n^5/15 + 341*n^4/9
 *             - 97*n^3/2 + 2341*n^2/72 - 87*n/10 + (n/2-1/2)*floor(n/2)
 *             = (1/360)*(15*n^8 - 240*n^7 + 1640*n^6 - 6168*n^5 + 13640*n^4
 *             - 17460*n^3 + 11705*n^2 - 3132*n + 180*floor(n/2)*(n-1))
 */

seq_t a(idx_t n)
{
    /* n must be >=1 for the sequence, but the function works for n==0 */
    if (n == 0)
        return 0;

    unsigned __int128 nn = n;          /* n as 128‑bit unsigned   */
    unsigned __int128 n2 = nn * nn;    /* n^2 */
    unsigned __int128 n3 = n2 * nn;    /* n^3 */
    unsigned __int128 n4 = n3 * nn;    /* n^4 */
    unsigned __int128 n5 = n4 * nn;    /* n^5 */
    unsigned __int128 n6 = n5 * nn;    /* n^6 */
    unsigned __int128 n7 = n6 * nn;    /* n^7 */
    unsigned __int128 n8 = n7 * nn;    /* n^8 */

    idx_t m = n >> 1;                  /* floor(n/2) */

    signed __int128 num = 0;

    num += ((signed __int128)n8) * 15;
    num -= ((signed __int128)n7) * 240;
    num += ((signed __int128)n6) * 1640;
    num -= ((signed __int128)n5) * 6168;
    num += ((signed __int128)n4) * 13640;
    num -= ((signed __int128)n3) * 17460;
    num += ((signed __int128)n2) * 11705;
    num -= ((signed __int128)nn) * 3132;
    num += ((signed __int128)m) * (signed __int128)(n - 1) * 180;

    /* division by 360 is exact for all integer n >= 1 */
    seq_t result = (seq_t)(num / 360);
    return result;
}
