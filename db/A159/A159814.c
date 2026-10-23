/*
 * A159814  Expansion of eta(z)^2*eta(4*z)^6/eta(2*z).
 * https://oeis.org/A159814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Euler transform of period‑4 sequence [-2, -1, -2, -7, …]
 * Range:     i8 n <= 9, i16 n <= 101, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) is the constant coefficient of the eta‑quotient product.          */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1; undefined for 0, return 0 safely   */
        return (seq_t)0;
    size_t idx = (size_t)(n - 1);          /* coefficient index (0‑based) */

    /* Static state for memoisation */
    static seq_t *coeff = NULL;            /* a[0 … max_n]  (coeff[0]=1)      */
    static seq_t *S     = NULL;            /* S[0 … max_n]   = Σ_{d|k} d*c_d  */
    static size_t coeff_cap = 0, max_n = 0;/* highest computed coefficient     */

    /* First call: initialise storage */
    if (!coeff) {
        coeff_cap = 1;
        coeff = (seq_t *)malloc(coeff_cap * sizeof(seq_t));
        if (!coeff) return (seq_t)0;
        coeff[0] = (seq_t)1;               /* constant term of the series    */
        S     = (seq_t *)malloc(coeff_cap * sizeof(seq_t));
        if (!S) return (seq_t)0;
        S[0] = 0;
    }

    /* If requested coefficient already computed, return it */
    if (idx <= max_n)
        return coeff[idx];

    /* ------------------------------------------------------------ */
    /* 1. Grow storage arrays to accommodate up to idx              */
    /* ------------------------------------------------------------ */
    size_t new_cap = coeff_cap;
    if (idx >= coeff_cap) {
        new_cap = coeff_cap ? coeff_cap << 1 : 1;
        while (new_cap <= idx) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(coeff, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        coeff = tmp;
        coeff_cap = new_cap;

        tmp = (seq_t *)realloc(S, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        S = tmp;
    }

    /* 2. Recompute S[1 … idx] from scratch (O(idx log idx))       */
    for (size_t i = 1; i <= idx; ++i)
        S[i] = 0;

    for (size_t d = 1; d <= idx; ++d) {
        /* period‑4 sequence c_d   (negative exponents)              */
        seq_t c_d;
        if ((d % 4) == 0)
            c_d = -7;
        else if ((d % 2) == 0)
            c_d = -1;
        else
            c_d = -2;
        seq_t term = c_d * (seq_t)d;          /* d * c_d      */

        for (size_t j = d; j <= idx; j += d)
            S[j] += term;
    }

    /* 3. Compute missing coefficients using the recurrence      */
    for (size_t m = max_n + 1; m <= idx; ++m) {
        seq_t sum = 0;
        for (size_t k = 1; k <= m; ++k)
            sum += S[k] * coeff[m - k];
        coeff[m] = sum / (seq_t)m;           /* exact division by m          */
    }

    max_n = idx;                            /* update memoised range        */
    return coeff[idx];
}
