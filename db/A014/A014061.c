/*
 * A014061  Inverse of 52nd cyclotomic polynomial.
 * https://oeis.org/A014061
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 if n = 0; for n > 0, * a(n) = - ∑_{k=1}^{24} c_k ·
 *            a(n‑k), * where c_k are the coefficients of the 52nd cyclotomic
 *            polynomial Φ₅₂(x). * The coefficients c_k are pre‑computed from
 *            (x^54 - x^52 - x² + 1) / * (x^30 - x^26 - x⁴ + 1) = Φ₅₂(x)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   compute_phi52_coeffs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for NULL */
#include <stdlib.h>   /* for malloc, realloc */
#include "../utils/compute_phi52_coeffs.h"

/* ------------------------------------------------------------------ */
/* Main sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* static storage for Φ₅₂ coefficients */
    static seq_t coeff[25];
    static int degree = -1;
    static int coeff_ready = 0;

    /* static cache for computed sequence terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of valid entries in cache */
    static idx_t cache_cap = 0;   /* allocated capacity */

    /* Initialize Φ₅₂ coefficients once */
    if (!coeff_ready) {
        compute_phi52_coeffs(coeff, &degree);
        coeff_ready = 1;
    }

    /* Ensure cache exists and contains the 0th term */
    if (!cache) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 1; /* a(0) = 1 */
        cache_len = 1;
    }

    /* Grow cache as needed and compute missing terms */
    while (cache_len <= n) {
        /* Grow storage when necessary */
        if (cache_len == cache_cap) {
            idx_t new_cap = cache_cap << 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute a(cache_len) using the recurrence */
        idx_t idx = cache_len;    /* current index to compute */
        seq_t sum = 0;
        int upper = (degree < (int)idx) ? degree : (int)idx;
        for (int k = 1; k <= upper; ++k) {
            sum += coeff[k] * cache[idx - k];
        }
        cache[idx] = -sum;

        ++cache_len;
    }

    return cache[n];
}
