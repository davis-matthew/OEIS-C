/*
 * A126924  Coordination sequence for 16-dimensional cyclotomic lattice
 *          Z[zeta_34].
 * https://oeis.org/A126924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0}^{16} p_k * binom(n-k+15, 15) where the
 *            coefficients p_k are p = [1, 18, 154, 834, 3214, 9402, 21778,
 *            41226, 65536, 41226, 21778, 9402, 3214, 834, 154, 18, 1]
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Coefficients of the numerator of the generating function */
    static const seq_t coeffs[17] = {
        (seq_t)1,   (seq_t)18,   (seq_t)154, (seq_t)834,
        (seq_t)3214, (seq_t)9402, (seq_t)21778, (seq_t)41226,
        (seq_t)65536, (seq_t)41226, (seq_t)21778, (seq_t)9402,
        (seq_t)3214, (seq_t)834,  (seq_t)154, (seq_t)18,
        (seq_t)1
    };

    seq_t result = 0;

    /* Only k up to min(n,16) contribute non‑zero binomial terms */
    idx_t maxk = (n < 16) ? n : (idx_t)16;

    for (idx_t k = 0; k <= maxk; ++k) {
        /* base = n - k */
        seq_t base = (seq_t)n - (seq_t)k;

        /* Compute binom(n-k+15, 15) multiplicatively:
           binom = product_{i=1}^{15} (base + i) / i   */
        seq_t binom = 1;
        for (int i = 1; i <= 15; ++i) {
            seq_t numerator = base + (seq_t)i;      /* base + i = n - k + i */
            binom = (binom * numerator) / (seq_t)i;
        }

        result += coeffs[k] * binom;
    }

    return result;
}
