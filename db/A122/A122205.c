/*
 * A122205  Number of finite sequences b with b(0) = 1, b(i+1) = b(i)+d where
 *          d|b(i), ending with n.
 * https://oeis.org/A122205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1 a(n) = Σ_{d|n, d<n} a(n-d) for n > 1 (OEIS A122205)
 * Range:     u8 n <= 14, u16 n <= 25, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

extern seq_t a(idx_t n); /* declared by the build system */

/* Cache of previously computed terms.  cache[0] is unused;
   cache[1] = 1.  The cache is grown on demand and memoised. */
static seq_t *cache = NULL;  /* pointer to cache array */
static size_t cache_cap = 0; /* current allocated capacity (>= cache_len+1) */
static idx_t cache_len = 0;  /* largest index computed so far */

/* Ensure that the cache can hold indices up to n.  Compute any
   missing terms. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_len)      /* already have needed terms */
        return;

    /* Grow the array if necessary.  We allocate at least n+1 slots. */
    size_t new_cap = (size_t)n + 1;
    if (new_cap > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            exit(EXIT_FAILURE);   /* allocation failed */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the cache if this is the first allocation. */
    if (cache_len == 0) {
        cache[0] = 0; /* unused entry */
        cache[1] = 1; /* a(1) = 1 */
        cache_len = 1;
    }

    /* Compute missing terms from cache_len+1 up to n. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t sum = 0;
        /* iterate over divisors d of i */
        for (idx_t d = 1; d <= i / d; ++d) {
            if (i % d == 0) {
                if (d < i) sum += cache[i - d];
                idx_t d2 = i / d;
                if (d2 != d && d2 < i) sum += cache[i - d2];
            }
        }
        cache[i] = sum;
    }
    cache_len = n;
}

seq_t a(idx_t n)
{
    if (n == 0)    /* out of range, defined as 0 */
        return 0;
    ensure_cache(n);
    return cache[n];
}
