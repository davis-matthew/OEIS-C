/*
 * A014067  Inverse of 58th cyclotomic polynomial.
 * https://oeis.org/A014067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=1; for n≥1: a(n) = -(c₁a(n‑1)+c₂a(n‑2)+…+c₂₈a(n‑28))
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where c₁…c₂₈ are the coefficients of the 58th cyclotomic polynomial Φ₅₈(x). */
#include <stdlib.h>

static void init_coeffs(seq_t coeff[29])
{
    /* numerator (x‑1)(x⁵⁸‑1)      : x⁶⁰ – x⁵⁹ – x + 1   */
    int N[60] = {0};
    N[0] = 1;
    N[1] = -1;         /* –x */
    N[58] = -1;        /* –x⁵⁸ */
    N[59] = 1;         /* +x⁵⁹ */

    /* denominator (x²‑1)(x²⁹‑1)  : x³¹ – x²⁹ – x² + 1  */
    int D[32] = {0};
    D[0] = 1;          /* +1 */
    D[2] = -1;         /* –x² */
    D[29] = -1;        /* –x²⁹ */
    D[31] = 1;         /* +x³¹ */

    int R[60];
    for (int i = 0; i < 60; ++i) R[i] = N[i];

    int Q[29] = {0};
    /* perform exact polynomial long division N ÷ D */
    for (int i = 59; i >= 31; --i) {
        int coef = R[i];
        if (coef != 0) {
            Q[i - 31] = coef;
            for (int j = 0; j <= 31; ++j) {
                R[(i - 31) + j] -= coef * D[j];
            }
        }
    }
    /* store coefficients (Q[0] is constant term) */
    for (int i = 0; i < 29; ++i) coeff[i] = (seq_t)Q[i];
}

seq_t a(idx_t n)
{
    /* ----------  compute coefficients of Φ58(x)  ---------- */
    static seq_t coeff[29];
    static int coeff_inited = 0;
    if (!coeff_inited) {
        init_coeffs(coeff);
        coeff_inited = 1;
    }

    /* ----------  cache of computed a(n)  ---------------- */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;   /* number of terms actually computed */

    size_t need = (size_t)n + 1;
    if (need > cache_cap) {
        size_t new_cap = need;
        seq_t *arr = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!arr) return 0;          /* allocation failed – fall back */
        cache = arr;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    if (cache_len <= (size_t)n) {
        if (cache_len == 0) {
            cache[0] = 1;           /* a(0) = 1  */
            cache_len = 1;
        }
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            if (i == 0) {
                cache[0] = 1;
            } else {
                seq_t sum = 0;
                size_t kmax = (i < 28 ? i : 28);
                for (size_t k = 1; k <= kmax; ++k) {
                    sum += coeff[k] * cache[i - k];
                }
                cache[i] = -sum;
            }
        }
        cache_len = (size_t)n + 1;
    }
    return cache[(size_t)n];
}
