/*
 * A161437  Number of reduced words of length n in the Weyl group A_5.
 * https://oeis.org/A161437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in *
 *            (1-x^2)(1-x^3)(1-x^4)(1-x^5)(1-x^6) / (1-x)^5 * (periodic up to
 *            n=15, after which a(n)=0)
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The numerator polynomial has degree 20; we need coefficients up to 20. */
    /* Index : coefficient */
    static const int coeff[21] = {
        1, 0, -1, -1, -1, 0, 0, 2, 2, 1,
        0, -1, -2, -2, 0, 0, 1, 1, 1, 0, -1
    };

    if (n > 15)          /* length of longest element in S_6 = 15 */
        return (seq_t)0;

    long long total = 0;

    for (idx_t k = 0; k <= n; ++k) {
        int c = coeff[k];
        if (c == 0) continue;

        idx_t t = n - k + 4;            /* t >= 4 for all k <= n */

        /* Compute (t choose 4) using seq_t arithmetic */
        seq_t bin = 1;
        bin = bin * (seq_t)(t - 3);
        bin = bin * (seq_t)(t - 2);
        bin = bin * (seq_t)(t - 1);
        bin = bin * (seq_t)(t);
        bin /= 24;                     /* 4! = 24 */

        total += (long long)c * (long long)bin;
    }

    return (seq_t)total;
}
