/*
 * A211155  Number of 2 X 2 matrices having all terms in {-n,...,0,..,n} and
 *          odd determinant.
 * https://oeis.org/A211155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 162, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS entry A211155: Number of 2×2 matrices having all terms in 
 * {-n,…,0,…,n} and odd determinant.
 *
 * Recurrence (for n > 9):
 *   a(n) = a(n-1) + 4*a(n-2) - 4*a(n-3) - 6*a(n-4) + 6*a(n-5)
 *          + 4*a(n-6) - 4*a(n-7) - a(n-8) + a(n-9)
 *
 * Initial values:
 *   a(0)=0, a(1)=40, a(2)=168, a(3)=1056, a(4)=2080,
 *   a(5)=6120, a(6)=9576, a(7)=20608, a(8)=28800, a(9)=52200
 *
 * The function below memoises computed values so that each call takes
 * time proportional to the smallest n that has not yet been computed.
 * It works for any seq_t/idx_t pair that satisfies the problem
 * constraints.
 */

#include <stddef.h>     /* size_t */
#include <stdlib.h>     /* malloc, realloc */
#include <stdint.h>     /* uint32_t, etc. */
#include <stdbool.h>    /* bool */

static seq_t *cache = NULL;          /* memoisation table */
static size_t cache_len = 0;          /* allocated size in elements */
static idx_t cache_computed = (idx_t)-1; /* highest n stored in cache */
static bool base_computed = false;   /* flag for initial 10 terms */

/* Compute the first ten values by brute force and store them in cache[0..9] */
static void compute_base(void)
{
    /* Ensure storage for at least 10 elements */
    if (cache_len < 10) {
        seq_t *new_cache = (seq_t *)malloc(10 * sizeof(seq_t));
        if (!new_cache) return;            /* allocation failure – nothing to do */
        if (cache) free(cache);
        cache = new_cache;
        cache_len = 10;
    }

    for (idx_t n = 0; n <= 9; ++n) {
        unsigned long long count = 0;    /* use a wider counter */
        int lim = (int)n;                /* entries run from -n to n */
        for (int a = -lim; a <= lim; ++a)
            for (int b = -lim; b <= lim; ++b)
                for (int c = -lim; c <= lim; ++c)
                    for (int d = -lim; d <= lim; ++d) {
                        int det = a * d - b * c;
                        if (det & 1) ++count;        /* odd determinant */
                    }
        cache[n] = (seq_t)count;
    }
    cache_computed = 9;
    base_computed  = true;
}

/* Grow the cache if necessary, keeping existing data. */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (!cache) {
        /* initialise cache (will also fill base values) */
        if (needed < 10) needed = 10;
        cache = (seq_t *)malloc(needed * sizeof(seq_t));
        if (!cache) return;           /* allocation failure */
        cache_len = needed;
        compute_base();
        return;
    }
    if (cache_len < needed) {
        seq_t *new_cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!new_cache) return;        /* allocation failure – keep old cache */
        cache = new_cache;
        cache_len = needed;
    }
}

/* Compute and store all values up to n, using the recurrence. */
static void compute_missing(idx_t n)
{
    for (idx_t i = cache_computed + 1; i <= n; ++i) {
        /* i must be >= 10 because we already have 0..9 */
        seq_t t =   cache[i-1]
                  + (seq_t)4 * cache[i-2]
                  - (seq_t)4 * cache[i-3]
                  - (seq_t)6 * cache[i-4]
                  + (seq_t)6 * cache[i-5]
                  + (seq_t)4 * cache[i-6]
                  - (seq_t)4 * cache[i-7]
                  - cache[i-8]
                  + cache[i-9];
        cache[i] = t;
    }
    cache_computed = n;
}

/* Public function required by the harness. */
seq_t a(idx_t n)
{
    if (!base_computed) compute_base();
    if (n <= (idx_t)9) return cache[n];
    ensure_cache(n);
    if (n > cache_computed) compute_missing(n);
    return cache[n];
}
