/*
 * A215628  Number of terms in 10th derivative of a function composed with
 *          itself n times.
 * https://oeis.org/A215628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n^7 +98*n^6 +3148*n^5 +40826*n^4 +203887*n^3
 *            +209204*n^2 -412380*n +136656) / 362880
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 39, u64 n <= 562, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* coefficients of the numerator polynomial Q(x) = -(4*x^8-34*x^7+110*x^6-161*x^5+83*x^4+30*x^3-32*x^2-x) */
    static const int64_t q[9] = {0, 1, 32, -30, -83, 161, -110, 34, -4};
    const unsigned __int128 denom = 362880u;
    signed __int128 acc = 0;

    for (int k = 0; k < 9; ++k) {
        int64_t coeff = q[k];
        if (coeff == 0) continue;

        idx_t m = n + 9 - (idx_t)k;   /* m = n - k + 9 */
        if (m < 9) continue;          /* C(m,9) = 0 for m < 9 */

        unsigned __int128 binom = 1;
        for (int i = 0; i < 9; ++i)
            binom *= (m - i);         /* compute m*(m-1)*...*(m-8) */

        binom /= denom;               /* divide by 9! */

        signed __int128 term = (signed __int128)binom * (signed __int128)coeff;
        acc += term;
    }
    return (seq_t)acc;
}
