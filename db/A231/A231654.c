/*
 * A231654  Number of non-equivalent ways to choose 5 points in an equilateral
 *          triangle grid of side n.
 * https://oeis.org/A231654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^10 + 5*n^9 - 10*n^8 - 70*n^7 + 25*n^6 + 584*n^5 -
 *            420*n^4 - 480*n^3 - 1216*n^2 + 1536*n + B) / 23040 where B =
 *            375*n^4 - 1170*n^3 + 210*n^2 - 405*n + 1035 if n is odd, and B =
 *            0 if n is even
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 24, u64 n <= 230, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Use unsigned __int128 for intermediate arithmetic to avoid
       overflow up to n≈6000 (which is the largest n that keeps
       the result within 64 bits). */
    unsigned __int128 u = n;
    unsigned __int128 s2 = u * u;
    unsigned __int128 s3 = s2 * u;
    unsigned __int128 s4 = s3 * u;
    unsigned __int128 s5 = s4 * u;
    unsigned __int128 s6 = s5 * u;
    unsigned __int128 s7 = s6 * u;
    unsigned __int128 s8 = s7 * u;
    unsigned __int128 s9 = s8 * u;
    unsigned __int128 s10 = s9 * u;

    /* Positive part of the numerator */
    unsigned __int128 Ppos =
        s10
        + 5 * s9
        + 25 * s6
        + 584 * s5
        + 1536 * u;

    /* Negative part of the numerator */
    unsigned __int128 Pneg =
        10 * s8
        + 70 * s7
        + 420 * s4
        + 480 * s3
        + 1216 * s2;

    /* B term, present only for odd n */
    unsigned __int128 B = 0;
    if (n & 1) {
        unsigned __int128 Bpos = 375 * s4 + 210 * s2 + 1035;
        unsigned __int128 Bneg = 1170 * s3 + 405 * u;
        B = Bpos - Bneg;          /* Bpos ≥ Bneg for odd n */
    }

    unsigned __int128 N = Ppos + B - Pneg;   /* full numerator */
    unsigned __int128 res = N / 23040;       /* exact division */

    return (seq_t)res;
}
