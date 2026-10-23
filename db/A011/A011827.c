/*
 * A011827  f-vectors for simplicial complexes of dimension at most 2 on at
 *          most n-1 vertices.
 * https://oeis.org/A011827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=0}^{6} q_i * C(n + 5 – i, 6) where q = (2, –11, 26,
 *            –32, 25, –7, 1) and C(m,6) is the binomial coefficient “m choose
 *            6”. This conversion is derived from the generating function
 *            –x*(x^6–7x^5+25x^4–32x^3+26x^2–11x+2)/(x–1)^7
 * Range:     u8 none, u16 n <= 4, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* value below offset is irrelevant */

    /* coefficients of the polynomial */
    static const int coeff[7] = { 2, -11, 26, -32, 25, -7, 1 };

    seq_t pos = 0;                     /* positive part of the sum   */
    seq_t neg = 0;                     /* negative part of the sum  */

    for (idx_t i = 0; i < 7 && i < n; ++i) {
        /* m = n + 5 – i  (always ≥ 6 for the loop condition) */
        idx_t m = n + 5 - i;

        /* compute C(m,6) = m*(m–1)*…*(m–5)/720 */
        seq_t bin = (seq_t)m;
        bin *= (m - 1);
        bin *= (m - 2);
        bin *= (m - 3);
        bin *= (m - 4);
        bin *= (m - 5);
        bin /= 720;                    /* exact division */

        int c = coeff[i];
        if (c > 0) {
            pos += bin * (seq_t)c;
        } else {
            neg += bin * (seq_t)(-c);
        }
    }

    /* pos ≥ neg for all n ≥ 1, so unsigned subtraction is safe */
    return pos - neg;
}
