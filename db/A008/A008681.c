/*
 * A008681  Expansion of 1/((1-x^3)*(1-x^4)*(1-x^5)*(1-x^6)).
 * https://oeis.org/A008681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^3+27*n^2+204*n+1440)/2160 + (n+6)*((2*n^2+1) mod
 *            3)/18 - n*(n mod 2)/48)
 * Range:     u8 none, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;

    /* term1 = n^3 + 27*n^2 + 204*n + 1440   */
    seq_t term1 = N * N * N + 27 * N * N + 204 * N + 1440;

    /* term2 = (n + 6) * ((2*n^2 + 1) mod 3) * 120   */
    seq_t modp = (2 * N * N + 1) % ((seq_t)3);
    seq_t term2 = (N + 6) * modp * 120;

    /* term3 = n * (n mod 2) * 45                    */
    seq_t term3 = N * (N & 1) * 45;

    /* numerator = term1 + term2 - term3              */
    seq_t num = (term1 + term2) - term3;

    /* result = numerator / 2160                      */
    seq_t result = num / 2160;
    return result;
}
