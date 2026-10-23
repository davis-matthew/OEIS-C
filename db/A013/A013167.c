/*
 * A013167  Expansion of e.g.f. log(arctanh(x) + exp(x)).
 * https://oeis.org/A013167
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   g_0 = 1 g_n = 1 if n is even g_n = 1 + (n-1)! if n is odd a(0) =
 *            0 For n ≥ 1: a_n = g_n - Σ_{k=1}^{n-1} binom(n-1,k-1) * a_k *
 *            g_{n-k} (i.e. a_{m+1} = g_{m+1} - Σ_{k=0}^{m-1} binom(m,k) *
 *            a_{k+1} * g_{m-k})
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 17, i128 n <= 18,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>           /* malloc, realloc */
#include "../utils/binom.h"   /* binom(idx_t a, idx_t b) */

static seq_t *cache_a = NULL;      /* a(n) values, indexed by n */
static seq_t *cache_g = NULL;      /* g(n) e.g.f. coefficients */
static seq_t *cache_fact = NULL;   /* factorial values stored for (n-1)! */
static size_t cache_size = 0;      /* number of slots allocated (>= max requested +1) */
static idx_t  a_comp_max = 0;      /* largest n for which a(n) has been computed */

/* Ensure that cache arrays are allocated up to index n (inclusive)*/
static void ensure_cache(idx_t n)
{
    if (cache_size > (size_t)n) return;           /* already large enough */

    size_t new_size = (size_t)n + 1;

    /* grow cache_a */
    if (cache_a == NULL) {
        cache_a = (seq_t *)malloc(new_size * sizeof(seq_t));
    } else {
        cache_a = (seq_t *)realloc(cache_a, new_size * sizeof(seq_t));
    }

    /* grow cache_g */
    if (cache_g == NULL) {
        cache_g = (seq_t *)malloc(new_size * sizeof(seq_t));
    } else {
        cache_g = (seq_t *)realloc(cache_g, new_size * sizeof(seq_t));
    }

    /* grow cache_fact */
    if (cache_fact == NULL) {
        cache_fact = (seq_t *)malloc(new_size * sizeof(seq_t));
    } else {
        cache_fact = (seq_t *)realloc(cache_fact, new_size * sizeof(seq_t));
    }

    /* initialise new entries */
    for (size_t i = cache_size; i < new_size; ++i) {
        if (i == 0) {
            cache_fact[0] = (seq_t)1;  /* (0)! = 1 */
            cache_g[0]    = (seq_t)1;  /* g_0 = 1 */
            cache_a[0]    = (seq_t)0;  /* a_0 = 0 */
        } else {
            /* factorial (i-1)! stored at fact[i] */
            cache_fact[i] = cache_fact[i-1] * (seq_t)i;
            /* g(i) */
            if (i & 1) {  /* odd */
                cache_g[i] = cache_fact[i-1] + (seq_t)1;
            } else {      /* even */
                cache_g[i] = (seq_t)1;
            }
        }
    }
    cache_size = new_size;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* make sure all needed data is stored */
    ensure_cache(n);

    /* compute a(n) values up to n if not already computed */
    if (a_comp_max < 1) {
        cache_a[1] = cache_g[1];          /* a_1 = g_1 */
        a_comp_max = 1;
    }

    for (idx_t m = a_comp_max; m < n; ++m) {
        /* we compute a_{m+1} for m = current to n-1 */
        seq_t tmp = cache_g[m+1];
        seq_t binom = (seq_t)1;          /* binom(m,0) */
        for (idx_t k = 0; k <= m-1; ++k) {
            seq_t sub = binom;
            sub *= cache_a[k+1];
            sub *= cache_g[m-k];
            tmp -= sub;

            /* update binom to binom(m, k+1) */
            binom = binom * (seq_t)(m - k);
            binom = binom / (seq_t)(k + 1);
        }
        cache_a[m+1] = tmp;
    }
    a_comp_max = n;
    return cache_a[n];
}
