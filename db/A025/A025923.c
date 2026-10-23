/*
 * A025923  Expansion of 1/((1-x^9)*(1-x^10)*(1-x^11)).
 * https://oeis.org/A025923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-9)+a(n-10)+a(n-11)-a(n-19)-a(n-20)-a(n-21)+a(n-30), *
 *            with a(0)=1 and a(n)=0 for 1≤n<9. This formulation uses a *
 *            static cache to evaluate any n in O(n) time irrespective of call
 *            order
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache of computed terms */
static seq_t *cache      = NULL;       /* array of seq_t terms */
static size_t cache_cap  = 0;          /* current capacity of the cache */
static idx_t  computed_max = (idx_t)-1;/* largest n that has been computed */

/* expand the cache to hold at least (n+1) elements (n is an idx_t) */
static void
ensure_cache(idx_t n)
{
    size_t new_cap = (size_t)n + 1;
    if (new_cap > cache_cap) {
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            /* allocation failed – terminate the program as the sequence
               cannot be computed without memory. */
            abort();
        }
        cache     = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (computed_max == (idx_t)-1) {
        ensure_cache(0);
        cache[0] = (seq_t)1;   /* a(0) = 1 */
        computed_max = 0;
    }

    /* make sure the cache is large enough for the requested index */
    if ((size_t)n >= cache_cap) {
        ensure_cache(n);
    }

    /* compute any missing values up to n */
    if (n > computed_max) {
        for (idx_t i = computed_max + 1; i <= n; ++i) {
            seq_t val = 0;
            if (i >= 9)  val += cache[i - 9];
            if (i >= 10) val += cache[i - 10];
            if (i >= 11) val += cache[i - 11];
            if (i >= 19) val -= cache[i - 19];
            if (i >= 20) val -= cache[i - 20];
            if (i >= 21) val -= cache[i - 21];
            if (i >= 30) val += cache[i - 30];
            cache[i] = val;
        }
        computed_max = n;
    }

    return cache[n];
}
