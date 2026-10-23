/*
 * A164536  a(n) = 10*a(n-1) - 23*a(n-2) for n > 1; a(0) = 3, a(1) = 23.
 * https://oeis.org/A164536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 23*a(n-2), a(0) = 3, a(1) = 23
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Sequence cache.  The array is grown lazily on the first call
 * and extended as needed for higher indices.  The cache holds
 * all values that have already been computed. */
static seq_t *seq_cache = NULL;
static idx_t  cache_upto = (idx_t)-1;   /* largest cached index (uninitialized -> -1) */
static size_t cache_cap  = 0;           /* current allocation size */

/* Initialise the cache with the two seed values. */
static void seq_init_cache(void) {
    if (seq_cache != NULL)
        return;
    cache_cap = 2;
    seq_cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    seq_cache[0] = 3;
    seq_cache[1] = 23;
    cache_upto = 1;
}

/* Ensure that the cache can address indices up to n.
 * The allocation size is enlarged if necessary. */
static void seq_ensure_capacity(idx_t n) {
    if ((size_t)n < cache_cap)
        return;
    size_t new_cap = n + 1;
    if (new_cap < cache_cap * 2)
        new_cap = cache_cap * 2;
    seq_cache = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Main entry point required by the judge. */
seq_t a(idx_t n) {
    if (n == 0) return 3;
    if (n == 1) return 23;

    seq_init_cache();
    if (n <= cache_upto)
        return seq_cache[n];

    seq_ensure_capacity(n);
    for (idx_t i = cache_upto + 1; i <= n; ++i) {
        /* recurrence: a(i) = 10*a(i-1) - 23*a(i-2) */
        seq_cache[i] = 10 * seq_cache[i-1] - 23 * seq_cache[i-2];
    }
    cache_upto = n;
    return seq_cache[n];
}
