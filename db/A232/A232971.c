/*
 * A232971  G.f.: (1-5*x+3*x^2+x^3)/(1-7*x+10*x^2+x^3-x^4).
 * https://oeis.org/A232971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 10*a(n-2) - a(n-3) + a(n-4) (n ≥ 4)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(0)=1, a(1)=2, a(2)=7, a(3)=29 */

#include <stdlib.h>

static seq_t *cache     = NULL;   /* cached sequence values */
static size_t  cache_cap = 0;     /* allocated capacity */
static idx_t   cache_len = 0;     /* number of values actually computed */

/* Ensure that cache contains terms up to index n (inclusive) */
static void
ensure_cache(idx_t n)
{
    if (cache_len > n)
        return;

    /* If cache is uninitialised, create it with first four terms */
    if (cache_len == 0) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) {
            /* In a real program you might handle this more gracefully */
            exit(EXIT_FAILURE);
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)7;
        cache[3] = (seq_t)29;
        cache_len = 4;
    }

    /* Grow and compute new terms until we reach n */
    while (cache_len <= n) {
        seq_t a_nm1 = cache[cache_len - 1];
        seq_t a_nm2 = cache[cache_len - 2];
        seq_t a_nm3 = cache[cache_len - 3];
        seq_t a_nm4 = cache[cache_len - 4];

        /* a(n) = 7*a(n-1) - 10*a(n-2) - a(n-3) + a(n-4) */
        seq_t val = (7 * a_nm1) - (10 * a_nm2) - a_nm3 + a_nm4;

        /* Expand cache if necessary */
        if ((size_t)cache_len >= cache_cap) {
            cache_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (!tmp) {
                exit(EXIT_FAILURE);
            }
            cache = tmp;
        }

        cache[cache_len++] = val;
    }
}

/* External linkage as required */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
