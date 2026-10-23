/*
 * A162596  G.f. is the polynomial (1-x^3) * (1-x^6) * (1-x^9) * (1-x^12) *
 *          (1-x^15) * (1-x^18) * (1-x^21) * (1-x^24) / (1-x)^8.
 * https://oeis.org/A162596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{T ⊆ {3,6,9,12,15,18,21,24}} (-1)^{|T|} * C(n - ΣT + 7,
 *            7)
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (C(·,·) is the binomial coefficient) */
seq_t a(idx_t n)
{
    /* Exponents appearing in the numerator product (1 - x^{3k}) for k = 1..8 */
    static const idx_t exps[8] = {3,6,9,12,15,18,21,24};
    seq_t result = 0;

    /* Iterate over all 256 subsets of the 8 exponents */
    for (unsigned mask = 0; mask < 256u; ++mask) {
        idx_t sum = 0;
        unsigned bits = 0;
        for (unsigned k = 0; k < 8; ++k) {
            if (mask & (1u << k)) {
                sum += exps[k];
                ++bits;
            }
        }

        if (n >= sum) {
            /* m = n - sum, we need C(m + 7, 7) */
            idx_t m = n - sum;

            /* Compute the binomial coefficient multiplicatively.
               C(m + 7, 7) = ∏_{i=1}^{7} (m + 8 - i) / i  */
            seq_t binom = 1;
            for (idx_t i = 1; i <= 7; ++i) {
                binom = binom * (m + 8 - i) / i;
            }

            /* Add or subtract according to parity of |T| */
            if (bits & 1u)
                result -= binom;
            else
                result += binom;
        }
    }
    return result;
}
