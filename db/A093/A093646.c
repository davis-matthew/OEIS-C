/*
 * A093646  Higher dimensional figurate numbers based on 12-gonal numbers
 *          A051624.
 * https://oeis.org/A093646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n+9)*C(n+8,8)/9 (C denotes binomial coefficient)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 25, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 27 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binom_n_plus8_choose_8(idx_t n)
{
    seq_t res = 1;
    idx_t N = n + 8;                     /* N = n+8 */
    /* Compute C(N,8) multiplicatively: (N-7)*(N-6)*...*N / 8! */
    for (idx_t i = 1; i <= 8; ++i) {
        res = res * (N - 8 + i);          /* multiply by a term of numerator */
        res = res / i;                    /* divide by i to keep it integer */
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t bin = binom_n_plus8_choose_8(n);
    seq_t coeff = (seq_t)(10 * n + 9);    /* 10*n+9 fits in idx_t for n up to limits */
    seq_t result = coeff * bin;
    result = result / 9;
    return result;
}
