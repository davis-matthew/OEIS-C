/*
 * A119020  Eigenvector of triangle A055151 of Motzkin polynomial
 *          coefficients, where A055151(n,k) = n!/((n-2k)!*k!*(k+1)!) for
 *          0<=k<=[n/2], n>=0.
 * https://oeis.org/A119020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} A055151(n,k)*a(k) * where A055151(n,k) =
 *            n! / ((n-2k)! * k! * (k+1)!) * and a(0) = 1. The recurrence is
 *            implemented iteratively and * memoised in static arrays. The
 *            triangle coefficients factor as * binom(n,2k)*Catalan(k), which
 *            is used to keep the computation * efficient...
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache of a(n) */
static seq_t *a_cache = NULL;
static idx_t a_cache_size = 0;
/* static cache of Catalan numbers Cat(k) */
static seq_t *cat_cache = NULL;
static idx_t cat_cache_size = 0;

seq_t a(idx_t n)
{
    /* initialise caches on first use */
    if (a_cache_size == 0) {
        a_cache = (seq_t *)realloc(a_cache, sizeof(seq_t));
        if (!a_cache) abort();
        a_cache[0] = 1;                 /* a(0) = 1 */
        a_cache_size = 1;
    }
    if (cat_cache_size == 0) {
        cat_cache = (seq_t *)realloc(cat_cache, sizeof(seq_t));
        if (!cat_cache) abort();
        cat_cache[0] = 1;                 /* Cat(0) = 1 */
        cat_cache_size = 1;
    }

    if (n < a_cache_size)            /* already computed */
        return a_cache[n];

    /* grow a_cache to required length */
    if (n + 1 > a_cache_size) {
        seq_t *tmp = (seq_t *)realloc(a_cache,
                                      sizeof(seq_t) * (n + 1));
        if (!tmp) abort();
        a_cache = tmp;
        /* the new entries will be overwritten immediately */
        a_cache_size = n + 1;
    }

    /* compute values from the first missing index up to n */
    for (idx_t i = a_cache_size - 1; i <= n; ++i) {

        /* ensure Catalan numbers up to floor(i/2) are available */
        idx_t maxk = i >> 1;   /* floor(i/2) */
        if (cat_cache_size <= maxk) {
            idx_t need = maxk + 1;
            seq_t *tmp = (seq_t *)realloc(cat_cache,
                                          sizeof(seq_t) * need);
            if (!tmp) abort();
            cat_cache = tmp;
            /* extend the cache from old size to new size */
            for (idx_t k = cat_cache_size; k < need; ++k) {
                if (k == 0) {
                    cat_cache[0] = 1;          /* Cat(0) */
                } else {
                    /* Cat(k) = Cat(k-1) * 2*(2k-1) / (k+1) */
                    seq_t num = cat_cache[k-1] *
                                ((seq_t)2 * (seq_t)(2 * k - 1));
                    cat_cache[k] = num / ((seq_t)(k + 1));
                }
            }
            cat_cache_size = need;
        }

        /* compute a(i) = sum_{k=0..floor(i/2)} binom(i,2k)*Cat(k)*a(k) */
        seq_t sum = 0;
        seq_t binom = 1;                   /* binom(i,0) */
        for (idx_t r = 0; r <= i; ++r) {
            if ((r & 1) == 0) {            /* even r => r = 2k */
                idx_t k = r >> 1;
                seq_t tmp = binom * cat_cache[k];
                seq_t term = tmp * a_cache[k];
                sum += term;
            }
            if (r == i) break;             /* finished last binomial */
            /* next binomial: binom(i, r+1) = binom * (i-r)/(r+1) */
            binom = binom * (i - r);
            binom = binom / (r + 1);
        }
        a_cache[i] = sum;
    }

    /* update cache size to reflect that we have written up to n */
    a_cache_size = n + 1;
    return a_cache[n];
}
