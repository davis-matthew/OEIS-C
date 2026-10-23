/*
 * A008352  a(n) is formed by concatenating a(n-2) and a(n-1), with a(0) = 1,
 *          a(1) = 2.
 * https://oeis.org/A008352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concat(a(n-2), a(n-1)) with a(0)=1, a(1)=2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   digit_len, pow10, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Utility headers providing the required helpers */
#include "../utils/seq_alloc_new_array.h"
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

/* Static cache for memoisation */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;          /* number of allocated slots */
static idx_t  max_idx    = 0;          /* highest index that has been computed */

/* Ensure the cache can hold at least (n+1) elements */
static void ensure_cache_capacity(idx_t n)
{
    if ((size_t)n < cache_cap) return;

    size_t new_cap = cache_cap;
    if (new_cap == 0) new_cap = 2;            /* minimal initial size */
    while ((size_t)n >= new_cap) new_cap *= 2;

    seq_t *new_cache = seq_alloc_new_array(new_cap);
    if (!new_cache) return; /* ignore failure – the caller will get a NULL pointer */

    if (cache) {
        for (size_t i = 0; i <= (size_t)max_idx; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }
    cache      = new_cache;
    cache_cap  = new_cap;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* Base values */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* Initialise cache on first use */
    if (!cache) {
        /* Allocate space for at least 2 terms */
        cache_cap = 2;
        cache = seq_alloc_new_array(cache_cap);
        if (!cache) return (seq_t)0; /* allocation failed – return zero */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        max_idx  = 1;
    }

    /* Return cached value if already computed */
    if (n <= max_idx) return cache[n];

    /* Make sure the cache is large enough */
    ensure_cache_capacity(n);

    /* Compute missing terms up to n */
    for (idx_t i = max_idx + 1; i <= n; ++i) {
        seq_t prev2 = cache[i - 2];
        seq_t prev1 = cache[i - 1];
        int d       = digit_len(prev1);               /* decimal digits of prev1 */
        seq_t pow   = pow10((idx_t)d);                /* 10^d */
        cache[i]    = prev2 * pow + prev1;            /* concatenation */
    }

    max_idx = n;
    return cache[n];
}
