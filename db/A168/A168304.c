/*
 * A168304  The fifth left hand column of triangle A167565.
 * https://oeis.org/A168304
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{j=0}^{4} c_j * binom(n - j + 4, 9) * with coefficient
 *            array c = {24, 128, 136, 32, 1}. Here * binom(v, 9) = v! / (9! *
 *            (v - 9)!), defined for v ≥ 9. * This implementation evaluates
 *            the binomial coefficient * multiplicatively to avoid
 *            intermediate overflow
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 27, u64 n <= 303, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binom9(idx_t v)
{
    /* binom(v,9);  v must be ≥ 9, otherwise result is 0 */
    if (v < 9) return 0;
    seq_t res = 1;
    for (idx_t i = 1; i <= 9; ++i) {
        res = res * (seq_t)(v - 9 + i);
        res = res / i;   /* exact division */
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 5) return 0;   /* offset is 5; values below are undefined */

    static const unsigned int coeff[5] = {24, 128, 136, 32, 1};
    seq_t result = 0;

    for (int j = 0; j < 5; ++j) {
        /* include term only when n-j+4 ≥ 9  <=> n ≥ j+5 */
        if (n >= (idx_t)(j + 5)) {
            idx_t v = n - j + 4;            /* v = n-j+4 >= 9 */
            seq_t b = binom9(v);            /* binom(v,9) */
            result += (seq_t)coeff[j] * b;  /* coefficient * binomial */
        }
    }
    return result;
}
