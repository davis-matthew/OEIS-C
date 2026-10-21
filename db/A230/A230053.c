/*
 * A230053  Recurrence a(n+2) = (n+2)*a(n+1)*a(n), with a(0) = a(1) = 1.
 * https://oeis.org/A230053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=1, a(n+2) = (n+2)*a(n+1)*a(n)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>               /* for size_t */
#include "../utils/seq_alloc_new_array.h" /* allocates seq_t array */
#include "../utils/seq_free.h"                /* frees seq_t array */

static seq_t *cache = NULL;          /* memoisation array */
static idx_t  cache_max = (idx_t)-1; /* largest index that has been cached */

/* Ensure that cache contains values up to at least n */
static void
ensure_cache(idx_t n)
{
    if (cache && n <= cache_max)
        return;                      /* already cached */

    /* Allocate new cache big enough for indices 0..n */
    size_t new_len = (size_t)(n + 1);
    seq_t *new_cache = seq_alloc_new_array(new_len);
    if (!new_cache)
        return;                      /* allocation failure – caller will crash correctly */

    /* Copy existing values if any */
    if (cache && cache_max != (idx_t)-1) {
        for (idx_t i = 0; i <= cache_max; ++i)
            new_cache[i] = cache[i];
        seq_free(cache);
    }

    cache = new_cache;
    cache_max = n;

    /* Initialise base cases */
    if (new_len > 0)
        cache[0] = (seq_t)1;
    if (new_len > 1)
        cache[1] = (seq_t)1;

    /* Compute remaining terms using the recurrence */
    for (idx_t i = 2; i <= n; ++i) {
        cache[i] = ((seq_t)i) * cache[i-1] * cache[i-2];
    }
}

/* External entry point – returns a(n) */
seq_t
a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
