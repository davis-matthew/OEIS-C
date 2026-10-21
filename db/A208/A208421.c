/*
 * A208421  Number of 4 X n 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 0 and 0 1 1 vertically.
 * https://oeis.org/A208421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 7*a(n-2) - 6*a(n-3) - 3*a(n-4) + 2*a(n-5).
 *            Initial values: a(1)=9, a(2)=81, a(3)=261, a(4)=1269, a(5)=5139
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for memoization (indices 0..n). */
static seq_t *cache = NULL;      /* Array storing computed terms. */
static idx_t cache_cap = 0;      /* Allocated capacity. */
static idx_t max_computed = 0;   /* Largest n for which cache[n] is valid. */

/* Pre‑computed initial values up to n = 5. */
static const seq_t init[6] = {0, 9, 81, 261, 1269, 5139};

/* Ensure that the cache has space for indices 0..n. */
static void ensure_cache(idx_t n)
{
    idx_t needed = n + 1;
    if (needed <= cache_cap)
        return;

    idx_t new_cap = cache_cap ? (cache_cap << 1) : 16;
    if (needed > new_cap)
        new_cap = needed;

    seq_t *ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!ptr)
        abort();

    cache = ptr;
    cache_cap = new_cap;
}

/* Return the n-th term (with offset 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* Off‑by‑one safety. */

    ensure_cache(n);

    if (n <= 5)
        return init[n];

    /* Initialise the cache for the first five terms if not already done. */
    if (max_computed < 5) {
        for (idx_t i = 1; i <= 5; ++i)
            cache[i] = init[i];
        max_computed = 5;
    }

    /* Compute missing terms using the empirical recurrence. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t val = cache[i - 1] * 3 + cache[i - 2] * 7
                    - cache[i - 3] * 6 - cache[i - 4] * 3
                    + cache[i - 5] * 2;
        cache[i] = val;
    }

    max_computed = n;
    return cache[n];
}
