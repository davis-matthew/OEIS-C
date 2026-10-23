/*
 * A156138  Q_{2n+1}(sqrt(2))/sqrt(2) (see A104035).
 * https://oeis.org/A156138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the coefficient of x^(2n+1)/(2n+1)! in \ sin(x)/(1-3
 *            sin(x)^2). The sequence satisfies \ (1-3 sin^2) * f = sin, with
 *            f(x) = A(x)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes the needed coefficients up to the
   requested odd index using an O(N^2) recurrence that avoids the
   explicit binomial convolution of sin^2.  */
#include <stdlib.h>

/* The types seq_t and idx_t are supplied by the build system.  */

seq_t a(idx_t n)
{
    static idx_t max_deg = 0;          /* maximum degree already computed */
    static seq_t *coeff = NULL;       /* array of f_k coefficients */

    /* we need coefficients up to index K = 2*n+1 (odd only) */
    const idx_t K = 2 * n + 1;

    if (K > max_deg) {
        /* free old storage if reallocating */
        if (coeff) free(coeff);
        coeff = (seq_t *)malloc(sizeof(seq_t) * (K + 1));
        if (!coeff) return 0; /* memory allocation failed */

        /* initial values: for odd indices only  */
        for (idx_t k = 0; k <= K; ++k) coeff[k] = 0;

        /* iterate over odd k = 1,3,5,...,K */
        for (idx_t k = 1; k <= K; k += 2) {
            const idx_t m = (k - 1) / 2;           /* k = 2*m+1          */
            const seq_t s_val = ((m & 1) == 0) ? (seq_t)1 : (seq_t)(-1);

            seq_t sum_pos = 0;
            seq_t sum_neg = 0;

            const idx_t j_max = m;                /* number of terms in sum */
            if (j_max >= 1) {
                /* initialise for j = 1: i = 2, binom = C(k,2) */
                seq_t binom = (seq_t)k * (seq_t)(k - 1) / 2; /* C(k,2)   */
                seq_t pow2 = (seq_t)2;                       /* 2^(2*1-1) */

                for (idx_t j = 1; j <= j_max; ++j) {
                    const idx_t i = 2 * j;                 /* even index i */
                    const idx_t prev = k - i;              /* k-2j  (odd) */
                    const seq_t f_prev = coeff[prev];

                    /* sign = (-1)^{j+1}  */
                    const seq_t sign = ((j & 1) == 0) ? (seq_t)(-1) : (seq_t)1;

                    /* term = C(k,i) * 2^(2j-1) * f_prev */
                    seq_t term = binom * pow2;
                    term = term * f_prev;

                    if (sign == (seq_t)1)
                        sum_pos += term;
                    else
                        sum_neg += term;

                    /* update binom to C(k, i+2) for next j */
                    if (j < j_max) {
                        const seq_t numerator =
                            (seq_t)(k - i) * (seq_t)(k - i - 1);
                        const seq_t denominator =
                            (seq_t)(i + 1) * (seq_t)(i + 2);
                        binom = (binom * numerator) / denominator;
                        pow2 <<= 2;          /* multiply by 4   */
                    }
                }
            }

            /* compute f_k = s_val + 3*(sum_pos - sum_neg) */
            const seq_t diff = sum_pos - sum_neg;          /* positive integer */
            seq_t val = diff * 3;
            if (s_val == (seq_t)1)
                val += 1;
            else
                val -= 1;

            coeff[k] = val;
        }

        max_deg = K;
    }

    return coeff[K];
}
