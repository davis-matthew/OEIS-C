/*
 * A008535  Coordination sequence for {A_7}* lattice.
 * https://oeis.org/A008535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{i=0}^{7} c_i * binom(n-i+6,6) where c =
 *            [1,9,37,93,93,37,9,1]
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 47, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: compute binomial coefficient binom(k,6) for k >= 0 */
static seq_t binom6(seq_t k)
{
    if (k < (seq_t)6) return (seq_t)0;
    seq_t b = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= (idx_t)6; ++i) {
        b = b * (k - (seq_t)(i - (idx_t)1)) / (seq_t)i;
    }
    return b;
}

seq_t a(idx_t n)
{
    static const seq_t coeffs[8] = {
        (seq_t)1, (seq_t)9, (seq_t)37, (seq_t)93,
        (seq_t)93, (seq_t)37, (seq_t)9, (seq_t)1
    };

    seq_t result = (seq_t)0;
    idx_t max_i = n < (idx_t)7 ? n : (idx_t)7;
    for (idx_t i = (idx_t)0; i <= max_i; ++i) {
        seq_t coeff = coeffs[i];
        seq_t k = (seq_t)(n - i + (idx_t)6);
        seq_t bin = binom6(k);
        result += coeff * bin;
    }
    return result;
}
