/*
 * A161876  Number of reduced words of length n in the Weyl group B_16.
 * https://oeis.org/A161876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   coefficients of ∏_{k=1}^{16} (1 + x + … + x^{2k-1})
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 21, u64 n <= 256, u128 n <= 256,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static seq_t coeff[257];
    static idx_t initialized = 0;

    if (!initialized) {
        /* initialise polynomial to 1 */
        for (idx_t i = 0; i < 257; ++i) coeff[i] = 0;
        coeff[0] = 1;
        idx_t current_max = 0;

        for (idx_t k = 1; k <= 16; ++k) {
            idx_t span     = 2 * k - 1;
            idx_t new_max  = current_max + span;

            seq_t temp[257];
            for (idx_t i = 0; i <= new_max; ++i) temp[i] = 0;

            for (idx_t i = 0; i <= current_max; ++i) {
                seq_t val = coeff[i];
                for (idx_t r = 0; r <= span; ++r) {
                    temp[i + r] = temp[i + r] + val;
                }
            }

            for (idx_t i = 0; i <= new_max; ++i) coeff[i] = temp[i];
            current_max = new_max;
        }

        initialized = 1;
    }

    if (n <= 256) return coeff[n];
    return (seq_t)0;
}
