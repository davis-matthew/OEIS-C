/*
 * A079046  Coefficients of the polynomials in the numerator of the generating
 *          function f(x)=(x-x^2)/(x^3-2x^2-2x+1) for F(n)^2, (where F(n) is
 *          the Fibonacci sequence) and its successive derivatives starting
 *          with the highest power of x.
 * https://oeis.org/A079046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf
 * Formula:   a(n) = coefficient of the numerator polynomial of the n-th
 *            derivative * of f(x) = (x - x^2)/(x^3 - 2x^2 - 2x + 1),
 *            flattened by descending powers * of x. The sequence is generated
 *            in O(n^2) time with small memory
 * Range:     i8 n <= 16, i16 n <= 38, i32 n <= 47, i64 n <= 47, i128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t Qcoeff[4]   = { 1, -2, -2, 1 };   /* Q(x) = 1 - 2x - 2x^2 + x^3   */
static const seq_t Qprime[3]   = { -2, -4, 3 };     /* Q'(x) = -2 - 4x + 3x^2      */

static seq_t *cache = NULL;            /* flat list of coefficients   */
static size_t cache_len = 0, cache_cap = 0;

static seq_t *poly = NULL;            /* current numerator polynomial (ascending) */
static size_t poly_len = 0;
static idx_t last_derivative = 0;

/* append a coefficient to the flat cache */
static void append_cache(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* compute the next derivative polynomial and extend the cache */
static void compute_next_derivative(void)
{
    idx_t n = last_derivative + 1;          /* derivative index of the new polynomial */
    size_t prev_len = poly_len;             /* length of previous polynomial */

    /* derivative of prev polynomial (ascending) */
    size_t der_len = prev_len - 1;
    seq_t *deriv = (seq_t*)malloc(der_len * sizeof(seq_t));
    for (size_t i = 0; i < der_len; ++i)
        deriv[i] = (seq_t)(i + 1) * poly[i + 1];

    size_t new_len = prev_len + 2;          /* length of new polynomial */

    seq_t *term1 = (seq_t*)calloc(new_len, sizeof(seq_t));
    seq_t *term2 = (seq_t*)calloc(new_len, sizeof(seq_t));

    /* term1 = deriv * Qcoeff */
    for (size_t i = 0; i < der_len; ++i)
        for (size_t j = 0; j < 4; ++j)
            term1[i + j] += deriv[i] * Qcoeff[j];

    /* term2 = poly * Qprime */
    for (size_t i = 0; i < prev_len; ++i)
        for (size_t j = 0; j < 3; ++j)
            term2[i + j] += poly[i] * Qprime[j];

    /* new polynomial = term1 - n * term2 */
    seq_t *new_poly = (seq_t*)malloc(new_len * sizeof(seq_t));
    for (size_t i = 0; i < new_len; ++i) {
        seq_t t1 = term1[i];
        seq_t t2 = (seq_t)n * term2[i];
        new_poly[i] = t1 - t2;
    }

    /* free temporary arrays */
    free(deriv);
    free(term1);
    free(term2);

    /* append new polynomial coefficients in descending order to cache */
    for (size_t i = new_len; i-- > 0; )
        append_cache(new_poly[i]);

    /* replace old polynomial with new one */
    free(poly);
    poly = new_poly;
    poly_len = new_len;
    ++last_derivative;
}

/* public function, returns the n-th term of the sequence */
seq_t a(idx_t n)
{
    /* initialisation on first call */
    if (cache_len == 0) {
        poly_len = 3;                       /* degree 2 polynomial */
        poly = (seq_t*)malloc(poly_len * sizeof(seq_t));
        poly[0] = 0;      /* x^0 term */
        poly[1] = 1;      /* x^1 term */
        poly[2] = -1;     /* x^2 term */

        /* cache the first polynomial in descending order */
        for (size_t i = poly_len; i-- > 0; )
            append_cache(poly[i]);
    }

    /* generate further polynomials until required index is covered */
    while (cache_len <= (size_t)n) {
        compute_next_derivative();
    }

    return cache[(size_t)n];
}
