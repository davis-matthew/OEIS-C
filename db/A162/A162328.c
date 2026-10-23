/*
 * A162328  Number of reduced words of length n in the Weyl group D_17.
 * https://oeis.org/A162328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 19, u64 n <= 272, u128 n <= 272,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = coefficient of x^n in 
 *      f(17) * ∏_{i=1}^{16} f(2i),
 * where f(m) = (1 – x^m)/(1 – x) = 1 + x + … + x^{m-1}.
 * The polynomial has degree 272, so a(n) = 0 for n > 272.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of polynomial coefficients */
    static seq_t *coeff   = NULL;
    static size_t coeff_len = 0;   /* 273 for D_17 */
    const unsigned K = 17;         /* Weyl group rank */

    if (!coeff) {                  /* first call – build the table */
        /* total degree = (K‑1) + Σ_{i=1}^{K‑1} (2i‑1) = 16 + 256 = 272 */
        size_t degree = (K - 1);
        for (unsigned i = 1; i < K; ++i) {
            degree += (2 * i - 1);
        }
        coeff_len = degree + 1;     /* 273 */

        /* start with the polynomial f(17) = 1 + x + … + x^{16} */
        coeff = (seq_t*)malloc(K * sizeof(seq_t));
        if (!coeff) return (seq_t)0;  /* out‑of‑memory guard */
        for (size_t i = 0; i < K; ++i) coeff[i] = 1;

        size_t curr_len = K;        /* current polynomial length */

        for (unsigned i = 1; i < K; ++i) {
            size_t f_len  = 2 * i;                     /* length of f(2i)  */
            size_t new_len = curr_len + f_len - 1;     /* length after convolution */

            /* allocate buffer for the new polynomial */
            seq_t *new_coeff = (seq_t*)malloc(new_len * sizeof(seq_t));
            if (!new_coeff) { free(coeff); return (seq_t)0; }

            /* initialize all coefficients to zero */
            for (size_t t = 0; t < new_len; ++t) new_coeff[t] = 0;

            /* convolution: each coefficient of f(2i) equals 1 */
            for (size_t a = 0; a < curr_len; ++a) {
                seq_t val = coeff[a];
                if (!val) continue;
                for (size_t b = 0; b < f_len; ++b) {
                    new_coeff[a + b] += val;
                }
            }

            /* replace the old polynomial */
            free(coeff);
            coeff     = new_coeff;
            curr_len  = new_len;
        }

        /* after the last iteration cur_len == coeff_len (273) */
    }

    /* answer the query */
    if (n >= (idx_t)coeff_len) return (seq_t)0;
    return coeff[(size_t)n];
}
