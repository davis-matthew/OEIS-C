/*
 * A158985  Coefficients of polynomials (in descending powers of x) P(n,x) :=
 *          1 + P(n-1,x)^2, where P(1,x) = x + 1.
 * https://oeis.org/A158985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 28, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A158985 – Coefficients of the polynomials
 *
 * P(1,x) = x + 1
 * P(n,x) = 1 + P(n-1,x)^2      (n ≥ 2)
 *
 * a(n) is the list of coefficients of P(1,x), P(2,x), … in
 * descending powers of x, row by row.
 *
 * Recurrence: square the previous polynomial, then add 1 to the
 * constant term.
 *
 * Initial values (a(1) … a(24)):
 * 1, 1, 1, 2, 2, 1, 4, 8, 8, 5, 1, 8, 32, 80, 138, 168, 144, 80, 26, 1,
 * 16, 128, 672, 2580 ... */

#include <stdlib.h>
#include <stddef.h>

/* -------------  cache for the flattened sequence  ------------- */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;

static void ensure_cache_cap(size_t needed)
{
    if (cache_cap == 0) {
        cache_cap = needed;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_cap < needed) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < needed) new_cap = needed;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

/* -------------  polynomial of the previous row  ------------- */
static seq_t *prev_poly = NULL;
static size_t prev_len  = 0;

/* -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* offset is 1, not used */

    size_t idx = (size_t)n;          /* n ≥ 1 */

    /* Build rows until the cache contains at least 'idx' coefficients */
    while (cache_len < idx) {
        if (prev_poly == NULL) {     /* first row: P(1,x) = x+1 */
            prev_len = 2;            /* degree 1 → two coeffs */
            prev_poly = (seq_t *)malloc(prev_len * sizeof(seq_t));

            prev_poly[0] = (seq_t)1; /* constant term */
            prev_poly[1] = (seq_t)1; /* coefficient of x */

            ensure_cache_cap(cache_len + prev_len);
            cache[cache_len++] = prev_poly[1];
            cache[cache_len++] = prev_poly[0];
        } else {                     /* subsequent rows */
            /* new polynomial: degree doubles, +1 */
            size_t new_len = 2 * prev_len - 1;
            seq_t *new_poly = (seq_t *)malloc(new_len * sizeof(seq_t));

            /* initialise to zero */
            for (size_t k = 0; k < new_len; ++k) new_poly[k] = (seq_t)0;

            /* square prev_poly (convolution) */
            for (size_t i = 0; i < prev_len; ++i) {
                seq_t ai = prev_poly[i];
                for (size_t j = i; j < prev_len; ++j) {
                    seq_t bj = prev_poly[j];
                    seq_t prod = ai * bj;
                    size_t pos = i + j;
                    if (i == j) {
                        new_poly[pos] += prod;
                    } else {
                        new_poly[pos] += prod * (seq_t)2;
                    }
                }
            }

            /* add the constant term +1 */
            new_poly[0] += (seq_t)1;

            /* free old polynomial & set up for next iteration */
            free(prev_poly);
            prev_poly = new_poly;
            prev_len  = new_len;

            /* append new polynomial's coefficients in descending order */
            ensure_cache_cap(cache_len + new_len);
            for (size_t k = new_len; k > 0; --k)
                cache[cache_len++] = new_poly[k - 1];
        }
    }

    return cache[idx - 1];
}
