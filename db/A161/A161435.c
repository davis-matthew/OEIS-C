/*
 * A161435  Number of reduced words of length n in the Weyl group A_3 (or
 *          D_3).
 * https://oeis.org/A161435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^n in (1 - x^2)(1 - x^3)(1 - x^4) / (1 -
 *            x)^3
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerator polynomial: 1 - x^2 - x^3 - x^4 + x^5 + x^6 + x^7 - x^9  */
seq_t a(idx_t n)
{
    /* Terms of the numerator polynomial; index positions and coefficients */
    static const int k_vals[8]   = {0, 2, 3, 4, 5, 6, 7, 9};
    static const int coeff[8]    = { 1, -1, -1, -1, 1, 1, 1, -1 };

    long long sum = 0;           /* accumulate in signed long long */

    for (int i = 0; i < 8; ++i) {
        idx_t k = (idx_t)k_vals[i];
        if (n < k) continue;                           /* binomial would be zero */

        /* m = n - k + 2; binomial coefficient C(m,2) = m*(m-1)/2 */
        idx_t m = n - k + 2;
        if (m < 2) continue;                           /* C(m,2)=0 for m<2 */

        long long bin = (long long)m * (long long)(m - 1) / 2;
        sum += coeff[i] * bin;
    }

    if (sum < 0) sum = 0;               /* safety: values vanish for n > 6 */
    return (seq_t)sum;
}
