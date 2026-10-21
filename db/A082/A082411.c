/*
 * A082411  Second-order linear recurrence sequence with a(n) = a(n-1) +
 *          a(n-2), with initial terms 407389224418, 76343678551.
 * https://oeis.org/A082411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 407389224418, a(1) = 76343678551
 * Range:     u8 none, u16 none, u32 none, u64 n <= 38, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;   /* cache of already computed terms   */
static size_t   cache_cap  = 0;   /* allocated size of seq_cache      */
static size_t   cache_len  = 0;   /* number of valid terms in cache   */

seq_t a(idx_t n)
{
    /* Base values (handled automatically by the cache initialization)   */
    const seq_t INIT0 = (seq_t)407389224418ULL;
    const seq_t INIT1 = (seq_t)76343678551ULL;

    /* Ensure that seq_cache is allocated with enough capacity.  The cache
       grows exponentially to keep amortised cost O(1) per added term.  */
    if (seq_cache == NULL) {
        /* Start with a small buffer; will be grown if necessary.          */
        cache_cap = 4;
        if ((size_t)n + 1 > cache_cap)
            cache_cap = (size_t)n + 1;
        seq_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (seq_cache == NULL) abort();
    } else if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        seq_cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first two terms on first use.                       */
    if (cache_len == 0) {
        seq_cache[0] = INIT0;
        if (cache_cap > 1) seq_cache[1] = INIT1;
        cache_len = (cache_cap > 1) ? 2 : 1;
    }

    /* If the requested term is already cached, return it directly.     */
    if ((size_t)n < cache_len)
        return seq_cache[(size_t)n];

    /* Otherwise, continue the recurrence up to n.                       */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_cache[i] = seq_cache[i - 1] + seq_cache[i - 2];
    }

    cache_len = (size_t)n + 1;
    return seq_cache[(size_t)n];
}
