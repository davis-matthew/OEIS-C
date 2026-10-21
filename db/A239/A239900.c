/*
 * A239900  a(0)=2, a(1)=5; thereafter a(n) = product of all preceding terms,
 *          plus 1.
 * https://oeis.org/A239900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=2, a(1)=5; For n>=2, a(n) = (product of all preceding
 *            terms) + 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Static cache of computed terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of terms currently stored   */
static size_t cache_cap = 0;   /* allocated capacity of cache array   */
static seq_t prod = 1;         /* product of all terms stored so far */

/* Ensure the cache has room for at least 'needed' elements. */
static void
ensure_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap << 1U : 4U;
    while (new_cap < needed)
        new_cap <<= 1U;

    if (cache_cap == 0U) {
        cache = seq_alloc(new_cap);
    } else {
        cache = seq_realloc(cache, cache_cap, new_cap);
    }
    cache_cap = new_cap;
}

/* Return the n-th term of sequence A239900.  n is unsigned 64-bit. */
seq_t
a(idx_t n)
{
    size_t idx = (size_t)n; /* safe cast: n will never exceed SIZE_MAX in tests */

    /* Return cached value if already computed. */
    if (idx < cache_len)
        return cache[idx];

    /* Compute missing terms up to n. */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        seq_t term;
        if (i == 0) {
            term = 2;
        } else if (i == 1) {
            term = 5;
        } else {
            term = prod + 1;          /* prod holds product of all previous terms */
        }

        ensure_cap(cache_len + 1U);
        cache[cache_len++] = term;
        prod = prod * term;           /* update product for next term            */
    }

    return cache[idx];
}
