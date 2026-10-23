/*
 * A135493  Number of ways to toss a coin n times and not get a run of six.
 * https://oeis.org/A135493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) + a(n-4) + a(n-5) a(0)=1,
 *            a(1)=2, a(2)=4, a(3)=8, a(4)=16, a(5)=32
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 65, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the n-th term of A135493 (connected to run‑of‑six coin tosses). */
seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;   /* pointer to cached values   */
    static size_t cap   = 0;      /* capacity of cache (in entries) */
    static size_t len   = 0;      /* number of values already computed */

    /* Convert n to a size_t for array indexing; it is safe for the tested range */
    size_t idx = (size_t)n;

    /* The cache must hold at least up to index n and at least 6 entries
       (the initial values). */
    size_t need = idx + 1;
    if (need < 6) need = 6;

    if (need > cap) {
        /* Reallocate cache.  For C++ we cast the void* returned by realloc. */
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failure: abort (build environment treats it as fatal). */
            abort();
        }
        cache = tmp;
        cap   = need;
    }

    /* Initialize the first six terms on first use. */
    if (len < 6) {
        const seq_t init[6] = { (seq_t)1, (seq_t)2, (seq_t)4,
                                (seq_t)8, (seq_t)16, (seq_t)32 };
        size_t i;
        for (i = 0; i < 6 && i < need; ++i) {
            cache[i] = init[i];
        }
        len = i;
    }

    /* Compute missing terms up to idx using the recurrence. */
    for (size_t i = len; i <= idx; ++i) {
        cache[i] = cache[i-1] + cache[i-2] + cache[i-3]
                 + cache[i-4] + cache[i-5];
    }
    len = idx + 1;

    return cache[idx];
}
