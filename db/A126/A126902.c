/*
 * A126902  Coordination sequence for 12-dimensional cyclotomic lattice
 *          Z[zeta_21].
 * https://oeis.org/A126902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{min(12,n)} c_k · C(n−k+11, 11), where c_k are
 *            the coefficients of the numerator in the generating function
 *            (x^12 + 9x^11 + 45x^10 + 158x^9 + 432x^8 + 909x^7 + 1302x^6 +
 *            909x^5 + 432x^4 + 158x^3 + 45x^2 + 9x + 1)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 16, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* numerator coefficients in descending power order */
    static const seq_t coeffs[13] = {
        (seq_t)1,  (seq_t)9,  (seq_t)45,  (seq_t)158, (seq_t)432,
        (seq_t)909, (seq_t)1302,(seq_t)909,(seq_t)432,(seq_t)158,
        (seq_t)45,  (seq_t)9,  (seq_t)1
    };

    seq_t result = (seq_t)0;

    for (idx_t i = 0; i < 13; ++i)
    {
        /* coefficient corresponds to x^(12-i) */
        idx_t k = 12 - i;
        if (k > n)
            continue;                /* term vanishes when n < k */

        /* m = n - k + 11 = n + i - 1  (≥ 11 for valid terms) */
        idx_t m = n + i - 1;
        if (m < 11)
            continue;

        /* compute binomial C(m, 11) multiplicatively */
        seq_t bin = (seq_t)1;
        for (idx_t j = 1; j <= 11; ++j)
        {
            bin = bin * ((seq_t)m - 11 + j);
            bin = bin / (seq_t)j;
        }

        result += coeffs[i] * bin;
    }

    return result;
}
