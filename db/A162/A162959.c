/*
 * A162959  The pairs (x,y) such that (x^2 + y^2)/(x*y + 1) is a perfect
 *          square, i.e., 4.
 * https://oeis.org/A162959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   a(n) = 4*a(n-2) - a(n-4), with a(1)=0, a(2)=2, a(3)=2, a(4)=8
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 35, u64 n <= 69, u128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#if defined(__cplusplus)
# include <cstdlib>
#else
# include <stdlib.h>
#endif

/* Recurrence implementation with memoization */
static seq_t *seq_cache      = NULL;   /* array of computed terms */
static idx_t  cache_max_idx  = 0;      /* highest index currently stored */
static size_t cache_capacity = 0;      /* allocated array size */

/* Ensure the cache contains at least terms up to idx n */
static void ensure_cache(idx_t n)
{
    if (n <= cache_max_idx)
        return;                       /* already computed */

    /* Grow the cache to size n+1 (index 0 unused) */
    size_t new_cap = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* Out‑of‑memory: terminate program */
        abort();
    }
    seq_cache = new_cache;
    cache_capacity = new_cap;

    /* Compute new terms */
    for (idx_t k = cache_max_idx + 1; k <= n; ++k) {
        if (k == 1)
            seq_cache[1] = 0;
        else if (k == 2)
            seq_cache[2] = 2;
        else if (k == 3)
            seq_cache[3] = 2;
        else if (k == 4)
            seq_cache[4] = 8;
        else
            seq_cache[k] = seq_cache[k - 2] * ((seq_t)4) - seq_cache[k - 4];
    }
    cache_max_idx = n;
}

seq_t a(idx_t n)
{
    if (n == 0)            /* sequence defined for n>=1, but 0 is unused   */
        return 0;
    ensure_cache(n);
    return seq_cache[n];
}
