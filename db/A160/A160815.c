/*
 * A160815  Expansion of (1+62*x+562*x^2+1023*x^3+458*x^4+49*x^5+x^6)/(1-x)^7.
 * https://oeis.org/A160815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 539*n^6/180 + 151*n^5/15 + 335*n^4/18 + 19*n^3 +
 *            2231*n^2/180 + 74*n/15 + 1 Equivalent to a(n) = Σ_{k=0}^{6} p_k
 *            * C(n-k+6, 6) with p = {1,62,562,1023,458,49,1}
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 33, u64 n <= 1353,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses only integer arithmetic on seq_t and idx_t. */

static seq_t binom6(idx_t m)  /* computes C(m, 6) for m >= 6 */
{
    if (m < 6)  /* value would be zero, but m is always >= 6 in use */
        return 0;

    seq_t res = 1;
    for (int i = 1; i <= 6; ++i) {
        /* multiply first, then divide by i – division is exact */
        res = res * (m - 6 + i) / i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* coefficients of the numerator polynomial: 1, 62, 562, 1023, 458, 49, 1 */
    static const unsigned int coeff[7] = {1, 62, 562, 1023, 458, 49, 1};

    seq_t result = 0;
    for (idx_t k = 0; k < 7; ++k) {
        if (n >= k) {
            idx_t t = n - k + 6;        /* argument for C(t, 6) */
            seq_t bin = binom6(t);      /* binomial coefficient */
            result += (seq_t)coeff[k] * bin;  /* multiply and add */
        }
    }
    return result;
}
