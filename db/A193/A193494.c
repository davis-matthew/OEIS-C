/*
 * A193494  Worst case of an unbalanced recursive algorithm over all n-node
 *          binary trees.
 * https://oeis.org/A193494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(0) = 0 * a(n) = a(n-1) + 2^(nu(n)-1) for n > 0 * where nu(n)
 *            = number of 1‑bits in the binary representation of n. * This
 *            recurrence yields a(n) = (A006046(n+1)-1)/2, as noted in OEIS
 * Range:     u8 n <= 54, u16 n <= 1865, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Static cache to store previously computed values.
 * cache_len always equals the number of elements stored (i.e. max index + 1). */
static seq_t *cache = NULL;
static idx_t   cache_len = 0;

/* Grow the cache if necessary and compute all missing values up to index n.
 * Uses the recurrence involving the popcount of each index. */
static void
ensure_cache_size(idx_t n)
{
    if (cache_len > n)          /* already have all needed values */
        return;

    idx_t old_len = cache_len;          /* first index not in the cache */
    idx_t new_len = n + 1;              /* total entries after growth */

    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    /* If realloc fails, leave the old cache unchanged – the
     * function will not be able to compute further values, but
     * this is unlikely to happen within the expected input range. */
    if (!new_cache)
        return;

    cache = new_cache;

    /* Compute missing entries */
    for (idx_t i = old_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)0;
        } else {
            unsigned bits = __builtin_popcountll((unsigned long long)i);
            seq_t add = ((seq_t)1 << (bits - 1));
            cache[i] = cache[i - 1] + add;
        }
    }

    cache_len = new_len;
}

seq_t a(idx_t n)
{
    ensure_cache_size(n);
    return cache[n];
}
