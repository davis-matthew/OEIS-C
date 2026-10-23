/*
 * A013333  sinh(exp(x)-sec(x)) = x + 2/3!*x^3 - 4/4!*x^4 + 12/5!*x^5 -
 *          120/6!*x^6 + ...
 * https://oeis.org/A013333
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in sinh(exp(x)-sec(x))
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 21, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A013333: a(0)=0, a(1)=1, a(2)=0, a(3)=2, a(4)=-4, a(5)=12, ... */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static storage for incremental computation */
    static seq_t *cache      = NULL;
    static seq_t *coeff_a    = NULL;   /* a_coeff (Factor for cos in sec inversion)   */
    static seq_t *coeff_b    = NULL;   /* b_coeff (sec series times factorial)       */
    static seq_t *coeff_d    = NULL;   /* d_coeff (y coefficients times factorial)    */
    static seq_t *exp_series = NULL;   /* e  coefficients of e^y, scaled by n!        */
    static seq_t *exp_neg    = NULL;   /* e- coefficients of e^{-y}, scaled by n!     */
    static idx_t  size_cached = 0;     /* current allocated size (valid indices 0..size_cached-1) */

    if (n + 1 > size_cached) {
        /* enlarge all arrays to new_size = n+1 */
        idx_t new_size = n + 1;
        cache      = (seq_t *)realloc(cache,      new_size * sizeof(seq_t));
        coeff_a    = (seq_t *)realloc(coeff_a,    new_size * sizeof(seq_t));
        coeff_b    = (seq_t *)realloc(coeff_b,    new_size * sizeof(seq_t));
        coeff_d    = (seq_t *)realloc(coeff_d,    new_size * sizeof(seq_t));
        exp_series = (seq_t *)realloc(exp_series, new_size * sizeof(seq_t));
        exp_neg    = (seq_t *)realloc(exp_neg,    new_size * sizeof(seq_t));

        /* initialise base case if first time */
        if (size_cached == 0) {
            cache[0]      = 0;
            coeff_a[0]    = 1;   /* not used but convenient */
            coeff_b[0]    = 1;
            coeff_d[0]    = 0;   /* unused */
            exp_series[0] = 1;
            exp_neg[0]    = 1;
        }

        /* Compute new entries starting from old size */
        idx_t start = size_cached;
        if (start == 0) start = 1;   /* index 0 is already initialised */

        for (idx_t i = start; i < new_size; ++i) {

            /* ----- 1. a_coeff: coefficients of cos in the sec inversion  ----- */
            if ((i & 1) == 0) {            /* even index */
                coeff_a[i] = ((i / 2) & 1) ? (seq_t)-1 : (seq_t)1;
            } else {
                coeff_a[i] = (seq_t)0;
            }

            /* ----- 2. b_coeff: sec(x) series times factorial ---------------- */
            seq_t sum_b = 0;
            seq_t binom = 1;   /* binom(i,0) */
            for (idx_t k = 1; k <= i; ++k) {
                binom = binom * (i - k + 1);
                binom = binom / k;
                if (coeff_a[k] != 0) {
                    sum_b += coeff_a[k] * coeff_b[i - k] * binom;
                }
            }
            coeff_b[i] = -sum_b;          /* integer by construction */

            /* ----- 3. d_coeff: y(x) coefficients times factorial (y=exp-sec) ----- */
            coeff_d[i] = (seq_t)1 - coeff_b[i];

            /* ----- 4. exp_series and exp_neg (e^y and e^{-y}) capped by n! ----- */
            seq_t sum_e    = 0;
            seq_t sum_eneg = 0;
            binom = 1;   /* binom(i,0) */
            for (idx_t k = 1; k <= i; ++k) {
                binom = binom * (i - k + 1);
                binom = binom / k;
                seq_t term = (seq_t)k * coeff_d[k];
                sum_e    += term * exp_series[i - k] * binom;
                sum_eneg += term * exp_neg[i - k]    * binom;
            }
            exp_series[i] = sum_e / (seq_t)i;
            exp_neg[i]    = - (sum_eneg / (seq_t)i);

            /* ----- 5. a(i) itself from the definition  --------------------- */
            cache[i] = (exp_series[i] - exp_neg[i]) / 2;
        }

        size_cached = new_size;
    }

    return cache[n];
}
