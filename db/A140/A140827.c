/*
 * A140827  Interleave denominators and numerators of convergents to sqrt(3).
 * https://oeis.org/A140827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-3) - a(n-6) for n >= 6, with initial values a(0)=1,
 *            a(1)=1, a(2)=2, a(3)=3, a(4)=4, a(5)=7. a(0) is the first term
 *            listed in the OEIS entry (offset 0)
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 51, u64 n <= 101,
 *            u128 n <= 202, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;           /* Cached values of the sequence. */
static idx_t cached_size = 0;         /* Number of valid entries in cache. */
static idx_t cached_cap  = 0;         /* Allocated capacity of cache. */

/* Ensure that cache has space for indices up to n (inclusive). */
static void ensure_cache_for(idx_t n)
{
    if (cached_cap > n) return;  /* Enough capacity already. */

    /* Grow capacity (double until enough). */
    idx_t new_cap = cached_cap ? cached_cap : 1;
    while (new_cap <= n) {
        new_cap <<= 1;
    }

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* In a real program we might handle allocation failure.  Here we
           simply abort since the exercise guarantees memory is available. */
        abort();
    }
    cache = new_cache;
    cached_cap = new_cap;
}

/* Compute missing terms up to n using the recurrence. */
static void compute_terms(idx_t n)
{
    if (cached_size > n) return;      /* Already computed. */

    /* Define the first 6 initial terms. */
    const seq_t init[6] = { (seq_t)1, (seq_t)1, (seq_t)2,
                            (seq_t)3, (seq_t)4, (seq_t)7 };

    /* If cache empty, initialize first 6 terms. */
    if (cached_size == 0) {
        ensure_cache_for(5);   /* Allocate for at least 6 elements. */
#ifdef __STDC_VERSION__
        memcpy(cache, init, sizeof(init));
#else
        for (idx_t i = 0; i < 6; ++i) cache[i] = init[i];
#endif
        cached_size = 6;
    }

    /* Ensure capacity for n. */
    ensure_cache_for(n);

    /* Compute terms from cached_size up to n. */
    for (idx_t i = cached_size; i <= n; ++i) {
        if (i < 6) continue;   /* Initial terms already set. */
        /* a(i) = 4*a(i-3) - a(i-6) */
        cache[i] = ( (seq_t)4 * cache[i - 3] ) - cache[i - 6];
    }

    cached_size = n + 1;
}

seq_t a(idx_t n)
{
    compute_terms(n);
    return cache[n];
}
