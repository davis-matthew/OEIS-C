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
 * Validated: standard, pass, 97 terms through n = 998 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for NULL */
#include <stdlib.h>   /* for malloc, realloc */
#include "../utils/mem.h"

/* Compute the coefficients of Φ₅₂(x) (degree 24) once.  Result is stored
 * in coeff[0..24] (coeff[0] == 1).  We also return the degree.  This
 * function is called lazily from a(). */
static void compute_phi52_coeffs(seq_t coeff[25], int *deg)
{
    /* Numerator: x^54 - x^52 - x^2 + 1 */
    seq_t num[55] = {0};
    num[54] = 1;
    num[52] = -1;
    num[2] = -1;
    num[0] = 1;

    /* Denominator: x^30 - x^26 - x^4 + 1 */
    seq_t den[31] = {0};
    den[30] = 1;
    den[26] = -1;
    den[4] = -1;
    den[0] = 1;

    /* Copy of numerator for in‑place division */
    seq_t rem[55];
    for (int i = 0; i < 55; ++i) rem[i] = num[i];

    /* Quotient will hold Φ₅₂(x) (degree 24) */
    seq_t quot[25] = {0};

    /* Long division: leading coefficient of den is 1 */
    for (int i = 54; i >= 30; --i) {
        seq_t q = rem[i];
        int qpos = i - 30;          /* position in quotient (0..24) */
        quot[qpos] = q;
        if (q != 0) {
            for (int j = 0; j <= 30; ++j) {
                rem[qpos + j] -= q * den[j];
            }
        }
    }

    /* Transfer quotient to output array */
    for (int i = 0; i <= 24; ++i)
        coeff[i] = quot[i];

    *deg = 24;   /* Φ₅₂ has degree 24 */
}

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
        cache = mem_new(seq_t, cache_cap);
        cache[0] = 1; /* a(0) = 1 */
        cache_len = 1;
    }

    /* Grow cache as needed and compute missing terms */
    while (cache_len <= n) {
        /* Grow storage when necessary */
        if (cache_len == cache_cap) {
            idx_t new_cap = cache_cap << 1;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
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
