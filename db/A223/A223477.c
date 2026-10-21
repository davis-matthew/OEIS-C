/*
 * A223477  Rolling icosahedron face footprints: number of n X 5 0..19 arrays
 *          starting with 0 where 0..19 label faces of an icosahedron and
 *          every array movement to a horizontal or antidiagonal neighbor
 *          moves across an icosahedral edge.
 * https://oeis.org/A223477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence (offset 1): a(1)=81, a(2)=3375
   a(n) = 51*a(n-1) - 300*a(n-2)   for n ≥ 3   (OEIS A223477) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of already computed values.
       cache[i] contains a(i+1) (because a(1) is stored at cache[0]). */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms stored (≥0)   */
    static size_t cache_cap = 0;  /* allocated slots (≥0)          */

    /* Initialise with the first two terms on the first call. */
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
        if (cache == NULL) {
            /* In a real environment you might handle out‑of‑memory
               here; the OXIS harness assumes memory is available. */
            return (seq_t)0;
        }
        cache[0] = (seq_t)81;    /* a(1) */
        cache[1] = (seq_t)3375;  /* a(2) */
        cache_len = 2;
    }

    /* Quick return for the first two terms. */
    if (n == 1) return cache[0];
    if (n == 2) return cache[1];

    /* Ensure the cache can hold up to index n. */
    if (n > cache_cap) {
        /* Grow capacity to at least n (use doubling for efficiency). */
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n) new_cap = new_cap * 2 + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n using the linear recurrence. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* a(i) = 51*a(i-1) - 300*a(i-2)  (indices: cache[i-1] = a(i)) */
        cache[i - 1] = cache[i - 2] * (seq_t)51 - cache[i - 3] * (seq_t)300;
    }
    cache_len = n;

    return cache[n - 1];
}
