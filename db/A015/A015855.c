/*
 * A015855  Inverse of 1846th cyclotomic polynomial.
 * https://oeis.org/A015855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   poly_div_xminus1, poly_mul_xminus1
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A015855 : Inverse of the 1846th cyclotomic polynomial
 *  The recurrence is derived from  Φ_1846(x) * Σ a(n) x^n = 1.
 *  Φ_1846(x) is a monic integer polynomial of degree 840.
 *  The coefficients of Φ_1846(x) are stored in b[0..deg].
 *
 *  a(0) = 1
 *  a(n) = - Σ_{k=1}^{min(n,deg)} b[k] * a(n-k)   for n ≥ 1
 *
 *  The sequence is periodic with period 1846.
 *
 *  The implementation computes the coefficients of Φ_1846(x) once
 *  using only additions, subtractions, and indexed copies.
 *  Then it evaluates the recurrence in O(n · deg) time, caching
 *  previously computed terms so that any order of queries is
 *  supported.
 *
 *  The algorithm is correct for any build where
 *      seq_t : an integer type (signed) of any width
 *      idx_t : unsigned 64‑bit integer (uint64_t)
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/poly_div_xminus1.h"
#include "../utils/poly_mul_xminus1.h"

/* Forward declaration: seq_t and idx_t are provided by the build */
extern seq_t a(idx_t n);

/* --------------------------------------------------------------------- */
/* 1.  Coefficients of Φ_1846(x)                                       */
/* --------------------------------------------------------------------- */

static int coeff_ready = 0;
static seq_t b[2000];   /* enough capacity for degree 840 */
static size_t deg = 0;   /* actual degree (will be 840) */

/* Compute the coefficients of Φ_1846 */
static void init_phi_coeffs(void)
{
    if (coeff_ready) return;

    /* Start with the polynomial 1 */
    size_t len = 1;
    seq_t coeff[2000];
    coeff[0] = (seq_t)1;

    /* Positive exponents: d = 2, 13, 71, 1846 (multiply) */
    const idx_t pos_d[] = {2, 13, 71, 1846};
    const int pos_cnt = 4;
    for (int j = 0; j < pos_cnt; ++j)
        poly_mul_xminus1(coeff, &len, (size_t)pos_d[j]);

    /* Negative exponents: d = 1, 26, 142, 923 (divide) */
    const idx_t neg_d[] = {1, 26, 142, 923};
    const int neg_cnt = 4;
    for (int j = 0; j < neg_cnt; ++j)
        poly_div_xminus1(coeff, &len, (size_t)neg_d[j]);

    /* At this point len should equal 840 (φ(1846)). */
    deg = len;
    for (size_t i = 0; i < len; ++i) b[i] = coeff[i];

    coeff_ready = 1;
}

/* --------------------------------------------------------------------- */
/* 2.  Cache for the sequence a(n)                                       */
/* --------------------------------------------------------------------- */

static seq_t *a_cache   = NULL;
static size_t a_cap     = 0;
static size_t a_len     = 0;

/* Make sure the cache has capacity at least “needed”. */
static void ensure_cache_capacity(size_t needed)
{
    if (a_cap >= needed) return;
    size_t new_cap = a_cap ? a_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_arr = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
    a_cache = new_arr;
    a_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/* 3.  The required function                                           */
/* --------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* Initialise data structures on first call */
    init_phi_coeffs();
    ensure_cache_capacity((size_t)n + 1);

    /* Ensure a(0) is defined */
    if (a_len == 0) {
        a_cache[0] = (seq_t)1;
        a_len = 1;
    }

    /* Compute missing terms up to n */
    if ((size_t)n >= a_len) {
        for (size_t k = a_len; k <= (size_t)n; ++k) {
            seq_t sum = (seq_t)0;
            size_t max_i = (k < deg) ? k : deg;
            for (size_t i = 1; i <= max_i; ++i) {
                sum += b[i] * a_cache[k - i];
            }
            a_cache[k] = -sum;
        }
        a_len = (size_t)n + 1;
    }
    return a_cache[(size_t)n];
}
