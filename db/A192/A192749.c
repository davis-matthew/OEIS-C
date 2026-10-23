/*
 * A192749  Constant term of the reduction by x^2->x+1 of the polynomial
 *          p(n,x) defined below in Comments.
 * https://oeis.org/A192749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence * a(n) = 3*a(n-1) -
 *            2*a(n-2) - a(n-3) + a(n-4) for n ≥ 4, * with initial values
 *            a(0)=0, a(1)=1, a(2)=6, a(3)=16
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for previously computed values */
static seq_t *cache = NULL;          /* array of computed a(n) */
static idx_t cache_len = 0;          /* number of values stored */
static size_t cache_cap = 0;         /* allocated capacity */

/* public function required by the harness */
seq_t a(idx_t n)
{
    /* ensure cache exists and contains at least the first four values */
    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();     /* fatal if allocation fails */

        cache[0] = 0;                  /* a(0) */
        cache[1] = 1;                  /* a(1) */
        cache[2] = 6;                  /* a(2) */
        cache[3] = 16;                 /* a(3) */
        cache_len = 4;
    }

    /* grow the cache if necessary */
    if (n >= cache_cap) {
        size_t new_cap = n + 1;        /* exact size needed */

        /* realloc may return NULL on failure; in that unlikely event abort */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) abort();

        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 4) {                   /* already set earlier */
            continue;
        }

        /* recurrence: a_i = 3*a_{i-1} - 2*a_{i-2} - a_{i-3} + a_{i-4}   */
        seq_t t = 3 * cache[i - 1];
        t += cache[i - 4];
        t -= 2 * cache[i - 2];
        t -= cache[i - 3];
        cache[i] = t;
    }

    cache_len = (n + 1 > cache_len) ? (n + 1) : cache_len;
    return cache[n];
}
