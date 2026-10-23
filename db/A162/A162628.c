/*
 * A162628  G.f. is the polynomial (1-x^3) * (1-x^6) * (1-x^9) * (1-x^12) *
 *          (1-x^15) * (1-x^18) * (1-x^21) * (1-x^24) * (1-x^27) * (1-x^30) *
 *          (1-x^33) / (1-x)^11.
 * https://oeis.org/A162628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the coefficient of x^n in the polynomial ∏_{k=1}^{11} (1
 *            + x + … + x^{3k-1}), i.e. a(n)=0 for n>187
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 42, u64 n <= 187, u128 n <= 187,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* the sequence is zero for n>187 */
    if (n > 187ull) return 0;

    /* cached polynomial coefficients (only computed once)          */
    static seq_t *coeff = NULL;
    static size_t coeff_len = 0;

    if (!coeff) {
        /* start with polynomial 1                     */
        coeff_len = 1;
        coeff = (seq_t*)malloc(sizeof(seq_t));
        coeff[0] = 1;

        /* multiply by each factor (1 + x + … + x^{3k-1}) */
        for (int k = 1; k <= 11; ++k) {
            size_t m = 3u * k;               /* length of the factor */
            size_t new_len = coeff_len + m - 1;

            seq_t *new_coeff = (seq_t*)calloc(new_len, sizeof(seq_t));
            if (!new_coeff) return 0; /* allocation failure */

            for (size_t i = 0; i < coeff_len; ++i) {
                seq_t val = coeff[i];
                /* add val to new_coeff[i .. i+m-1] */
                for (size_t j = 0; j < m; ++j) {
                    new_coeff[i + j] += val;
                }
            }

            free(coeff);
            coeff = new_coeff;
            coeff_len = new_len;
        }
    }

    return coeff[(size_t)n];
}
