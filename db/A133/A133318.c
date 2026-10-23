/*
 * A133318  Dimensions of certain Lie algebra (see reference for precise
 *          definition).
 * https://oeis.org/A133318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{j=0}^{9} c[j] * C(n - j + 17, 17) where c = [1, 48,
 *            603, 2898, 6174, 6174, 2898, 603, 48, 1] (derived from the
 *            empirical generating function)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 10, u64 n <= 50, u128 n <= 737,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binomial_u64(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Coefficients from the numerator (x+1)*P(x) of the empirical g.f. */
    static const int coeffs[10] = {1, 48, 603, 2898, 6174, 6174,
                                   2898, 603, 48, 1};
    seq_t result = 0;
    for (int j = 0; j < 10; ++j) {
        if (n >= (idx_t)j) {
            idx_t t = n - (idx_t)j + 17;
            seq_t bin = binomial_u64(t, 17);
            if (bin != 0) {
                result = result + (seq_t)coeffs[j] * bin;
            }
        }
    }
    return result;
}
