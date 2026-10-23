/*
 * A014394  Inverse of 385th cyclotomic polynomial.
 * https://oeis.org/A014394
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) are the coefficients of the formal power series 1/Φ_385(x),
 *            where Φ_385 is the 385th cyclotomic polynomial. The sequence is
 *            periodic with period 385. The coefficients are computed by the
 *            linear recurrence a(0)=1, a(n)= -∑_{i=1}^{min(n,240)} φ_i *
 *            a(n-i) where φ_i are the coefficients of Φ_38...
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   multiply_by_xd_minus1
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes */
#include <stdlib.h>
#include <stddef.h>
#include "../utils/multiply_by_xd_minus1.h"

/* Helper: polynomial division by monic divisor.
   Compute quotient q = num / div   (exact division).   */
static void poly_div_monic(const seq_t *num, size_t lnum,
                           const seq_t *div, size_t ldiv,
                           seq_t *quo, size_t *lquo)
{
    size_t deg_num = lnum - 1;
    size_t deg_div = ldiv - 1;
    size_t deg_quo = deg_num - deg_div;
    size_t qlen = deg_quo + 1;
    seq_t *rem = (seq_t *)calloc(lnum, sizeof(seq_t));
    if (!rem) return; /* allocation failure */

    /* copy numerator to remainder */
    for (size_t i = 0; i < lnum; ++i) rem[i] = num[i];

    for (long long i = (long long)deg_num; i >= (long long)deg_div; --i) {
        seq_t q = rem[i];
        long long qidx = i - deg_div;
        quo[qidx] = q;
        if (q) {
            for (size_t j = 0; j <= deg_div; ++j) {
                rem[qidx + j] -= q * div[j];
            }
        }
    }
    /* remainder should be zero */
    free(rem);
    *lquo = qlen;
}

/* Compute Φ_385 coefficients once */
static void init_phi_385(seq_t *phi, size_t *phi_len)
{
    static int init_done = 0;
    if (init_done) return;

    /* positive factors: 5,7,11,385  (μ=+1) */
    const size_t plus_ds[] = {5,7,11,385};
    /* negative factors: 1,35,55,77 (μ=-1) */
    const size_t minus_ds[] = {1,35,55,77};

    /* Polynomial buffers */
    seq_t P_plus[500] = {0};
    seq_t P_minus[500] = {0};

    size_t len_plus = 1, len_minus = 1;
    P_plus[0] = 1;
    P_minus[0] = 1;

    /* multiply plus factors */
    for (size_t k = 0; k < sizeof(plus_ds)/sizeof(plus_ds[0]); ++k) {
        multiply_by_xd_minus1(P_plus, &len_plus, plus_ds[k]);
    }
    /* multiply minus factors */
    for (size_t k = 0; k < sizeof(minus_ds)/sizeof(minus_ds[0]); ++k) {
        multiply_by_xd_minus1(P_minus, &len_minus, minus_ds[k]);
    }

    /* divide: φ = P_plus / P_minus (exact, P_minus monic) */
    poly_div_monic(P_plus, len_plus, P_minus, len_minus, phi, phi_len);

    init_done = 1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* static storage for φ coefficients */
    static seq_t phi[241];
    static size_t phi_len = 0;

    /* static cache for a(n) values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;      /* number of valid entries (cache_len = last index +1) */
    static size_t cache_cap = 0;      /* allocated capacity */

    /* initialise φ  */
    if (phi_len == 0) {
        init_phi_385(phi, &phi_len);          /* phi_len should become 241 */
    }

    /* ensure cache has value for a(0) */
    if (cache_len == 0) {
        cache_cap = 64;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) return 0;                /* allocation failure, return 0 */
        cache[0] = 1;                        /* a(0) = 1 */
        cache_len = 1;
    }

    /* extend cache up to n */
    if ((idx_t)cache_len <= n) {
        /* grow cache if necessary */
        while (cache_len <= (size_t)n) {
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *new_mem = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_mem) return 0;     /* allocation failure */
                cache = new_mem;
                cache_cap = new_cap;
            }

            /* compute a(cache_len) */
            size_t idx = cache_len;
            seq_t sum = 0;
            size_t max_j = idx < (phi_len - 1) ? idx : (phi_len - 1);
            for (size_t j = 1; j <= max_j; ++j) {
                sum += phi[j] * cache[idx - j];
            }
            cache[idx] = -sum;
            ++cache_len;
        }
    }

    return cache[n];
}
