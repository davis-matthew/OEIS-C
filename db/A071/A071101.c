/*
 * A071101  Expansion of (5 + 6*x + 3*x^2 - 2*x^3) / (1 - 2*x - 2*x^2 - 2*x^3
 *          + x^4) in powers of x.
 * https://oeis.org/A071101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=5, a(1)=16, a(2)=45, a(3)=130; a(n) = 2*a(n-1) + 2*a(n-2) +
 *            2*a(n-3) - a(n-4) for n ≥ 4
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 40, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Remember the cache of computed values. */
static seq_t *cache = NULL;      /* pointer to cached terms */
static size_t cache_cap = 0;     /* current capacity of cache */
static size_t cache_len = 0;     /* number of valid entries in cache */

/* Helper: make sure the cache can hold at least n+1 terms. */
static void ensure_cache(idx_t n)
{
    /* If already large enough, nothing to do. */
    if (n + 1 < cache_cap)
        return;

    /* Determine new capacity: at least n+1, double each step for amortized O(1). */
    size_t new_cap = cache_cap ? cache_cap : 4;
    while (new_cap <= (size_t)n + 1)
        new_cap <<= 1;

    /* Allocate new storage. */
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* In a production environment you might handle this error. */
        abort();
    }

    /* Copy existing values. */
    for (size_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];

    /* Free old cache and update pointers/capacity. */
    free(cache);
    cache = new_cache;
    cache_cap = new_cap;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n < 4) {
        /* Initial values. */
        static const seq_t init[4] = {5, 16, 45, 130};
        return init[n];
    }

    /* Ensure cache is allocated and large enough. */
    if (!cache)
        cache = (seq_t *)malloc(4 * sizeof(seq_t));  /* initial capacity 4 */
    if (!cache)
        abort();  /* allocation failure */

    if (cache_len == 0) {
        /* Initialise cache[0..3] with the first four terms. */
        cache[0] = 5;
        cache[1] = 16;
        cache[2] = 45;
        cache[3] = 130;
        cache_len = 4;
    }

    /* Grow cache if necessary. */
    ensure_cache(n);

    /* Compute missing terms up to n. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t val = 2 * cache[i-1] + 2 * cache[i-2]
                  + 2 * cache[i-3] - cache[i-4];
        cache[i] = val;
    }
    cache_len = (size_t)n + 1;  /* we now have values 0..n */

    return cache[(size_t)n];
}
