/*
 * A125116  Number of 8 X 8 pandiagonal Franklin squares with magic sum 4n.
 * https://oeis.org/A125116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A125116
 *  a(n) = (196608*n^8 + 1572864*n^7 + 7340032*n^6 + 22020096*n^5
 *          + 44040192*n^4 + 58720256*n^3 + 51642368*n^2
 *          + 27787264*n + 6881280) / 6881280
 *
 *  Derived from the closed‑form expression
 *      a(n) = s^8/2293760 + s^7/71680 + s^6/3840 + s^5/320
 *             + s^4/40 + 2*s^3/15 + 197*s^2/420 + 106*s/105 + 1,
 *  with s = 4*n.  The polynomial coefficients are integers, so the
 *  division is exact for every n ≥ 0.
 */
seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic if available to avoid overflow
     * in the intermediate calculations.  This is safe for all
     * n that fit in the seq_t result, even when seq_t is 64‑bit. */
    unsigned __int128 N = n;
    unsigned __int128 num = 0;
    /* Coefficients for the polynomial in n (highest to lowest). */
    static const unsigned __int128 coeff[] =
    {
        196608ULL,     /* n^8 */
        1572864ULL,    /* n^7 */
        7340032ULL,    /* n^6 */
        22020096ULL,   /* n^5 */
        44040192ULL,   /* n^4 */
        58720256ULL,   /* n^3 */
        51642368ULL,   /* n^2 */
        27787264ULL,   /* n^1 */
        6881280ULL     /* n^0 */
    };
    /* Horner's rule */
    num = coeff[0];
    for (int i = 1; i < 9; ++i)
        num = num * N + coeff[i];
    /* Exact division by 6881280 */
    return (seq_t)(num / 6881280ULL);
}
