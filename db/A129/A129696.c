/*
 * A129696  Antidiagonal sums of triangular array T defined in A014430: T(j,k)
 *          = binomial(j+1, k) - 1 for 1 <= k <= j.
 * https://oeis.org/A129696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-1) + a(n-2) + 1 + floor((n-1)/2),
 *           a(1) = 1, a(2) = 2
 *
 * This implementation memoises the sequence values so that
 * subsequent calls with arbitrary indices are O(1).
 * It uses a dynamically growing array allocated with malloc.
 */
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Forward declaration of the function to be implemented */
seq_t a(idx_t n);

/* Memoisation storage */
static seq_t *cache      = NULL;    /* array of cached values */
static idx_t   cache_cap = 0;      /* capacity of the array (in elements) */
static idx_t   last      = 0;      /* largest index that has been computed */

/* Helper to grow the cache if necessary */
static void ensure_cache_cap(idx_t needed)
{
    if (needed < cache_cap) return;

    /* Determine new capacity (at least needed+1) */
    idx_t new_cap = (cache_cap == 0) ? 8 : cache_cap << 1;
    while (new_cap <= needed) new_cap <<= 1;

    seq_t *new_ptr = (seq_t *)malloc(sizeof(seq_t) * (size_t)new_cap);
    if (!new_ptr) {
        /* In an actual project we would handle the error. */
        exit(1);
    }

    if (cache) {
        /* Copy the old values to the new block */
        memcpy(new_ptr, cache, sizeof(seq_t) * (size_t)cache_cap);
        free(cache);
    }

    cache      = new_ptr;
    cache_cap  = new_cap;
}

seq_t a(idx_t n)
{
    /* Guard against invalid n (offset starts at 1) */
    if (n == 0) return 0; /* undefined, but return 0 */

    /* Initialise cache on first call and set base cases */
    if (!cache) {
        /* Allocate enough space (at least 3 elements for a(1), a(2)) */
        idx_t init_cap = (n >= 2) ? (n + 1) : 3;
        cache = (seq_t *)malloc(sizeof(seq_t) * (size_t)init_cap);
        if (!cache) exit(1);
        cache_cap = init_cap;

        /* Base values a(1) and a(2) */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        last = (n >= 2) ? 2 : 1;
    }

    /* Grow cache if needed */
    if (n >= cache_cap) {
        ensure_cache_cap(n);
    }

    /* Compute missing values up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* a(i) = a(i-1) + a(i-2) + 1 + floor((i-1)/2) */
        seq_t term = (seq_t)((i - 1) / 2) + 1;
        cache[i] = cache[i - 1] + cache[i - 2] + term;
    }
    last = n;

    return cache[n];
}
