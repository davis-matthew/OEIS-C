/*
 * A170869  a(0) = 1, a(1) = 2; a(2^i + j) = 3a(j) + 2a(j + 1) for 0 <= j <
 *          2^i.
 * https://oeis.org/A170869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=2; a(2^i + j) = 3*a(j) + 2*a(j+1) for 0 ≤ j < 2^i
 * Range:     u8 n <= 13, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes A170869 using a bottom‑up memoised
 * approach that works for any seq_t size (including arbitrary
 * precision).  It uses a static cache that grows only when a
 * larger index is requested.  The implementation is fully
 * compatible with C11 and C++17. */
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;      /* cache for computed terms */
static idx_t cache_len = 0u;     /* number of entries in cache */

/* Allocate or extend the cache up to at least "n" (inclusive). */
static void
_ensure_cache(idx_t n)
{
    if (n + 1u <= cache_len) return;          /* already big enough */

    size_t new_len = (size_t)(n + 1u);
    seq_t *new_cb = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cb) abort();                     /* allocation failure */
    cache = new_cb;

    /* Zero initialise new entries; this is safe for unsigned types and
     * for arbitrary‑precision types that support zero construction. */
    for (size_t i = cache_len; i < new_len; ++i) cache[i] = (seq_t)0;

    cache_len = new_len;
}

seq_t a(idx_t n)
{
    /* Handle the two fixed initial values. */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)2;

    /* Make sure the cache can hold indices up to n. */
    idx_t old_len = cache_len;
    _ensure_cache(n);

    /* Initialise the cache with the known starting values if they are
     * not already present. */
    if (old_len == 0u) {         /* cache empty */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        old_len = 2u;
    } else if (old_len == 1u) {  /* only a(0) was present */
        cache[1] = (seq_t)2;
        old_len = 2u;
    }

    /* Compute all missing terms up to n. */
    for (idx_t i = old_len; i <= n; ++i) {
        /* Identify the largest power of two not exceeding i.  For
         * i > 0 the expression below is safe.  (i == 0 is handled
         * earlier.) */
        unsigned int log2i = 63u - __builtin_clzll((unsigned long long)i);
        idx_t pow2 = (idx_t)1 << log2i;

        idx_t j = i - pow2;      /* 0 ≤ j < pow2 */

        /* Note that j+1 ≤ pow2 ≤ i‑1, so cache[j] and cache[j+1]
         * have already been computed. */
        cache[i] = (seq_t)3 * cache[j] + (seq_t)2 * cache[j + 1];
    }

    return cache[n];
}
