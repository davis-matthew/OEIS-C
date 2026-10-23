/*
 * A161715  a(n) = (50*n^7 - 1197*n^6 + 11333*n^5 - 53655*n^4 + 132125*n^3 -
 *          156828*n^2 + 73212*n + 5040)/5040.
 * https://oeis.org/A161715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 28*a(n-2) + 56*a(n-3) - 70*a(n-4) * +
 *            56*a(n-5) - 28*a(n-6) + 8*a(n-7) - a(n-8) * Initial values:
 *            a(0)=1, a(1)=2, a(2)=3, a(3)=5, a(4)=6, * a(5)=10, a(6)=15,
 *            a(7)=30
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 49, u64 n <= 1096,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached sequence values */
static seq_t *cache = NULL;      /* array of computed terms */
static idx_t cache_size = 0;     /* allocated size (count of elements) */
static idx_t last_computed = (idx_t)(-1); /* index of the last computed value */

/* Ensure the cache can hold indices up to `max_index` (inclusive). */
static void ensure_cache(idx_t max_index)
{
    /* Number of elements needed */
    idx_t required = max_index + 1;
    if (required <= cache_size)
        return;                 /* already large enough */

    if (required < 8)          /* keep a minimal size for the initial block */
        required = 8;

    seq_t *new_cache = (seq_t *)malloc(required * sizeof(seq_t));
    if (!new_cache)                /* allocation failed – keep old state */
        return;

    /* Copy existing data if any */
    if (cache) {
        for (idx_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }

    cache = new_cache;
    cache_size = required;
}

/* Return the `n`‑th term of OEIS A161715. */
seq_t a(idx_t n)
{
    /* Handle the first eight terms directly */
    if (n < 8) {
        static const seq_t init[8] = {1, 2, 3, 5, 6, 10, 15, 30};
        return init[(size_t)n];
    }

    /* Initialise the cache on the first call */
    if (!cache) {
        ensure_cache(7);
        cache[0] = 1; cache[1] = 2; cache[2] = 3; cache[3] = 5;
        cache[4] = 6; cache[5] = 10; cache[6] = 15; cache[7] = 30;
        last_computed = 7;
    }

    /* Grow the cache if needed */
    if (n >= cache_size)
        ensure_cache(n);

    /* Compute any missing values up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t val = 8 * cache[i - 1];
        val -= 28 * cache[i - 2];
        val += 56 * cache[i - 3];
        val -= 70 * cache[i - 4];
        val += 56 * cache[i - 5];
        val -= 28 * cache[i - 6];
        val += 8 * cache[i - 7];
        val -= cache[i - 8];
        cache[i] = val;
    }
    last_computed = n;

    return cache[n];
}
