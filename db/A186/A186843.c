/*
 * A186843  Number of (n+1) X 3 binary arrays with no 2 X 2 subblock diagonal
 *          sum less antidiagonal sum equal to any horizontal or vertical
 *          neighbor 2 X 2 subblock diagonal sum less antidiagonal sum.
 * https://oeis.org/A186843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 28, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula implemented: a(n) = 2*a(n-1) + 4*a(n-2) + 24*a(n-3)
                            + 14*a(n-4) + 3*a(n-5) + 2*a(n-6)
   Initial values: a(1)=50, a(2)=218, a(3)=970, a(4)=4194,
                   a(5)=18246, a(6)=79778. */

#include <stdlib.h>
#include <stdbool.h>

/* Cached computed values.
   cache[0] is unused to keep indices 1‑based. */
static seq_t *cache     = NULL;  /* array of computed terms */
static size_t cache_len = 0;     /* highest index currently stored */
static size_t cache_cap = 0;     /* allocated capacity (in terms) */

/* The six known starting terms. */
static const seq_t init_vals[7] = {0ULL,
    50ULL, 218ULL, 970ULL, 4194ULL, 18246ULL, 79778ULL};

/* Ensure that cache contains values up to index n (>=1). */
static void
ensure_cache(idx_t n)
{
    if ((idx_t)cache_len >= n) return;          /* already computed */

    /* At least n+1 positions are needed (index 0 unused). */
    size_t new_cap = (cache_cap ? cache_cap : 16);
    while (new_cap <= (size_t)n) new_cap <<= 1;   /* double until enough */

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);                            /* allocation failed */
    cache = tmp;
    cache_cap = new_cap;

    /* Compute new terms. */
    for (idx_t i = (idx_t)cache_len + 1; i <= n; ++i) {
        if (i <= 6) {
            cache[i] = init_vals[i];
        } else {
            cache[i] =
                2 * cache[i-1] +
                4 * cache[i-2] +
                24 * cache[i-3] +
                14 * cache[i-4] +
                3 *  cache[i-5] +
                2 *  cache[i-6];
        }
    }
    cache_len = (size_t)n;
}

/* Public function: returns a(n) for n >= 1. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* convention for n < offset */
    ensure_cache(n);
    return cache[n];
}
