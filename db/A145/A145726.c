/*
 * A145726  Expansion of q * psi(-q) * psi(-q^15) / (psi(-q^3) * psi(-q^5)) in
 *          powers of q where psi() is a Ramanujan theta function.
 * https://oeis.org/A145726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   Expansion of q * ψ(-q) * ψ(-q¹⁵) / (ψ(-q³) * ψ(-q⁵)) where ψ(q)
 *            = Σ q^{n(n+1)/2} (n≥0). ψ(-q) means replace q by -q in the
 *            series, i.e. ψ(-q) = Σ (-1)^{n(n+1)/2} q^{n(n+1)/2}
 * Range:     i8 n <= 97, i16 n <= 277, i32 n <= 865, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   psi_neg_series, series_inv, series_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/psi_neg_series.h"
#include "../utils/series_inv.h"
#include "../utils/series_mul.h"

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* static cache */
    static seq_t *psi1   = NULL;
    static seq_t *psi15  = NULL;
    static seq_t *psi3   = NULL;
    static seq_t *psi5   = NULL;
    static seq_t *den    = NULL;
    static seq_t *invden = NULL;
    static seq_t *num    = NULL;
    static seq_t *shift  = NULL;
    static seq_t *final  = NULL;
    static size_t cache_size = 0;

    size_t N = (size_t)n;          /* need coefficients 0..N inclusive */

    if (N > cache_size) {
        /* free old arrays */
        free(psi1);  free(psi15);  free(psi3);  free(psi5);
        free(den);   free(invden); free(num);   free(shift); free(final);

        /* allocate new arrays */
        psi1   = (seq_t *)calloc(N + 1, sizeof(seq_t));
        psi15  = (seq_t *)calloc(N + 1, sizeof(seq_t));
        psi3   = (seq_t *)calloc(N + 1, sizeof(seq_t));
        psi5   = (seq_t *)calloc(N + 1, sizeof(seq_t));
        den    = (seq_t *)calloc(N + 1, sizeof(seq_t));
        invden = (seq_t *)calloc(N + 1, sizeof(seq_t));
        num    = (seq_t *)calloc(N + 1, sizeof(seq_t));
        shift  = (seq_t *)calloc(N + 1, sizeof(seq_t));
        final  = (seq_t *)calloc(N + 1, sizeof(seq_t));

        /* compute the four ψ series */
        psi_neg_series(1,  N, psi1);    /* ψ(-q)   */
        psi_neg_series(15, N, psi15);  /* ψ(-q¹⁵) */
        psi_neg_series(3,  N, psi3);    /* ψ(-q³)  */
        psi_neg_series(5,  N, psi5);    /* ψ(-q⁵)  */

        /* denominator = ψ(-q³) * ψ(-q⁵) */
        series_mul(psi3, psi5, den, N);

        /* inverse of denominator */
        series_inv(den, N, invden);

        /* numerator = ψ(-q) * ψ(-q¹⁵) */
        series_mul(psi1, psi15, num, N);

        /* shift numerator by q (multiply by q) */
        shift[0] = 0;
        for (size_t i = 0; i < N; ++i) shift[i + 1] = num[i];

        /* final series = (q * ψ(-q) * ψ(-q¹⁵)) / (ψ(-q³) * ψ(-q⁵)) */
        series_mul(shift, invden, final, N);

        cache_size = N;
    }

    return final[n];
}
