/*
 * A118403  Unsigned row sums of triangle A118401; a(n) = A118402(n^2-n+2),
 *          where A118402 is the row sums of triangle A118400.
 * https://oeis.org/A118403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{i=0..4} c_i * binomial(n - i + 2, 2) with
 *            coefficients c = {1, -2, 3, -2, 2}. This follows from the
 *            generating function A(x) = (1-2x+2x^2)*(1+x^2)/(1-x)^3
 * Range:     u8 n <= 17, u16 n <= 257, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* coefficients for the numerator polynomial (1-2x+2x^2)*(1+x^2) */
    static const int coeff[5] = {1, -2, 3, -2, 2};

    long long result = 0;
    for (int i = 0; i < 5; ++i) {
        if (n < (idx_t)i) continue;                  /* term vanishes for n < i */
        /* compute k = n - i + 2 as seq_t, then convert to long long */
        seq_t sn   = (seq_t)n;
        seq_t k_se = sn - (seq_t)i + (seq_t)2;
        long long k  = (long long)k_se;              /* k is at most ~10000+2 */
        long long binom = k * (k - 1) / 2;           /* binomial(k,2) */
        result += (long long)coeff[i] * binom;
    }
    return (seq_t)result;
}
