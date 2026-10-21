/*
 * A097852  Expansion of (1+x^15)/((1-x^2)*(1-x^4)*(1-x^6)*(1-x^10)).
 * https://oeis.org/A097852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n^3+33*n^2+324*n+1440 - (n mod 2)*(n^2+7*n+18)*45) /
 *            1440)
 * Range:     u8 none, u16 n <= 29, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    // convert index to seq_t for arithmetic
    seq_t nn = (seq_t)n;

    /* Compute the numerator: n^3 + 33*n^2 + 324*n + 1440 */
    seq_t n2   = nn * nn;
    seq_t n3   = n2 * nn;
    seq_t term1 = n3 + 33 * n2 + 324 * nn + 1440;

    /* Subtract the odd‑n correction term when n is odd */
    seq_t odd_corr = 0;
    if ((nn & 1) != 0) {
        odd_corr = (n2 + 7 * nn + 18) * 45;   /* (n^2+7n+18)*45  */
    }

    seq_t numerator = term1 - odd_corr;

    /* Final division by 1440 with truncating integer division gives the floor */
    return numerator / 1440;
}
