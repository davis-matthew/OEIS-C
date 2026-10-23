/*
 * A015104  Inverse of 1095th cyclotomic polynomial.
 * https://oeis.org/A015104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   divide_by_product, multiply_arrays
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; for n≥1,
   a(n) = –∑_{i=1}^{576} Φ_1095[i] * a(n−i) ,
   where Φ_1095[i] are the coefficients of the 1095th cyclotomic polynomial. */
#include <stdlib.h>
#include <string.h>
#include "../utils/divide_by_product.h"
#include "../utils/multiply_arrays.h"

/* Global storage for the 1095th cyclotomic polynomial coefficients. */
static seq_t  g_pi1095[600] = {0};
static size_t g_pi1095_size = 0;
static int   phi_computed = 0;

/* Compute Φ_1095 and fill g_pi1095[]. */
static void compute_phi1095(void)
{
    if (phi_computed)
        return;

    /* φ1 : x – 1 */
    seq_t *phi1 = (seq_t *)calloc(2, sizeof(seq_t));
    phi1[0] = (seq_t)-1;
    phi1[1] = (seq_t)1;

    /* φ3 = (x^3 – 1) / φ1 */
    seq_t *phi3 = divide_by_product(3, phi1, 2);

    /* φ5 = (x^5 – 1) / φ1 */
    seq_t *phi5 = divide_by_product(5, phi1, 2);

    /* φ15 = (x^15 – 1) / (φ1 * φ3 * φ5) */
    seq_t *tmp1 = multiply_arrays(phi1, 2, phi3, 3);          /* size 4 */
    seq_t *tmp2 = multiply_arrays(tmp1, 4, phi5, 5);          /* size 8 */
    free(tmp1);
    seq_t *phi15  = divide_by_product(15, tmp2, 8);           /* size 9 */
    free(tmp2);

    /* φ73 = 1 + x + … + x^72 */
    seq_t *phi73 = (seq_t *)calloc(73, sizeof(seq_t));
    for (size_t i = 0; i < 73; ++i)
        phi73[i] = (seq_t)1;

    /* φ219 = (x^219 – 1) / (φ1 * φ3 * φ73) */
    seq_t *tmp3 = multiply_arrays(phi1, 2, phi3, 3);          /* size 4 */
    seq_t *tmp4 = multiply_arrays(tmp3, 4, phi73, 73);        /* size 76 */
    free(tmp3);
    seq_t *phi219 = divide_by_product(219, tmp4, 76);         /* size 145 */
    free(tmp4);

    /* φ365 = (x^365 – 1) / (φ1 * φ5 * φ73) */
    seq_t *tmp5 = multiply_arrays(phi1, 2, phi5, 5);          /* size 6 */
    seq_t *tmp6 = multiply_arrays(tmp5, 6, phi73, 73);        /* size 78 */
    free(tmp5);
    seq_t *phi365 = divide_by_product(365, tmp6, 78);         /* size 289 */
    free(tmp6);

    /* product of all proper divisors of 1095 */
    seq_t *prodA = multiply_arrays(phi1, 2, phi3, 3);          /* size 4 */
    seq_t *prodB = multiply_arrays(prodA, 4, phi5, 5);         /* size 8 */
    free(prodA);
    seq_t *prodC = multiply_arrays(prodB, 8, phi15, 9);        /* size 16 */
    free(prodB);
    seq_t *prodD = multiply_arrays(prodC, 16, phi73, 73);      /* size 88 */
    free(prodC);
    seq_t *prodE = multiply_arrays(prodD, 88, phi219, 145);    /* size 232 */
    free(prodD);
    seq_t *prodF = multiply_arrays(prodE, 232, phi365, 289);   /* size 520 */
    free(prodE);

    seq_t *phi1095 = divide_by_product(1095, prodF, 520);      /* size 577 */
    free(prodF);

    /* copy to global buffer */
    g_pi1095_size = 577;
    for (size_t i = 0; i < g_pi1095_size; ++i)
        g_pi1095[i] = phi1095[i];

    /* free all temporaries */
    free(phi1); free(phi3); free(phi5); free(phi15);
    free(phi73); free(phi219); free(phi365);
    free(phi1095);

    phi_computed = 1;
}

/* Dynamic storage for the sequence values a(n). */
static seq_t *a_cache  = NULL;
static size_t a_cache_size = 0;   /* number of values currently stored */
static size_t a_cache_cap  = 0;   /* allocated capacity */

/* Ensure a_cache has space for indices up to needed-1. */
static void ensure_cache(size_t needed)
{
    if (needed <= a_cache_cap)
        return;
    size_t new_cap = a_cache_cap ? a_cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_ptr = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!new_ptr) abort();   /* memory exhausted */
    /* zero the new part to avoid garbage reads */
    memset(new_ptr + a_cache_cap, 0, (new_cap - a_cache_cap) * sizeof(seq_t));
    a_cache = new_ptr;
    a_cache_cap = new_cap;
}

/* Main function required by the specification. */
seq_t a(idx_t n)
{
    /* initialise our static data on first call */
    if (!phi_computed)
        compute_phi1095();
    if (a_cache_size == 0) {
        ensure_cache(1);
        a_cache[0] = (seq_t)1;      /* a(0) = 1 */
        a_cache_size = 1;
    }

    if (n < (idx_t)a_cache_size)
        return a_cache[(size_t)n];

    /* extend cache up to index n */
    size_t start = a_cache_size;
    ensure_cache((size_t)n + 1);
    for (size_t k = start; k <= (size_t)n; ++k) {
        seq_t sum = (seq_t)0;
        size_t lim = k < g_pi1095_size - 1 ? k : g_pi1095_size - 1;
        for (size_t i = 1; i <= lim; ++i)
            sum += g_pi1095[i] * a_cache[k - i];
        a_cache[k] = -sum;
    }
    a_cache_size = (size_t)n + 1;
    return a_cache[(size_t)n];
}
