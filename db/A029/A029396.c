/*
 * A029396  Expansion of 1/((1-x^5)*(1-x^7)*(1-x^8)*(1-x^12)).
 * https://oeis.org/A029396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^3+48*n^2+540*n-3254)/20160 * + ((n mod
 *            4)-2)^2*(n+36)/192 * + ((n mod 4) - ((n+3) mod 4))/16 * +
 *            ((2*n^3+n^2+2) mod 5)/5 * + ((2*n^3+5*n^2+2*n+2) mod 7)/7)
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use signed 64‑bit arithmetic for intermediate values */
    int64_t nn = (int64_t)n;

    /* Term 1: (n^3 + 48*n^2 + 540*n - 3254) / 20160   */
    int64_t N1 = nn * nn * nn + 48 * nn * nn + 540 * nn - 3254;

    /* Term 2: ((n mod 4)-2)^2*(n+36)/192, with LCM 20160 => * 105 */
    int64_t m4 = nn & 3;          /* n mod 4 */
    int64_t diff2 = m4 - 2;
    int64_t N2 = diff2 * diff2 * (nn + 36) * 105LL;

    /* Term 3: ((n mod 4) - ((n+3) mod 4))/16, 20160/16 = 1260 */
    int64_t diff3 = m4 - ((nn + 3) & 3);
    int64_t N3 = diff3 * 1260LL;

    /* Term 4: ((2*n^3 + n^2 + 2) mod 5)/5, 20160/5 = 4032 */
    int64_t mod5 = (2 * nn * nn * nn + nn * nn + 2) % 5;
    int64_t N4 = mod5 * 4032LL;

    /* Term 5: ((2*n^3 + 5*n^2 + 2*n + 2) mod 7)/7, 20160/7 = 2880 */
    int64_t mod7 = (2 * nn * nn * nn + 5 * nn * nn + 2 * nn + 2) % 7;
    int64_t N5 = mod7 * 2880LL;

    /* Sum all numerators and divide by 20160 */
    int64_t sum = N1 + N2 + N3 + N4 + N5;
    seq_t result = (seq_t)(sum / 20160LL);
    return result;
}
