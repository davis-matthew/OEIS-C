/*
 * A161203  n-th square plus n-th squarefree number.
 * https://oeis.org/A161203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   is_squarefree, seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = n^2 + A005117(n)
 *          where A005117(n) is the n-th square‑free number.
 */
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_squarefree.h"
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

static seq_t *sqfree_cache = NULL;          /* cached square‑free numbers   */
static size_t cache_len   = 0;              /* number of cached entries     */
static size_t cache_cap   = 0;              /* allocated capacity           */
static idx_t  last_checked = 0;             /* last integer examined        */

/* Ensure the array has at least 'need' slots, reallocating if required. */
static void ensure_cache_capacity(size_t need)
{
    if (cache_cap >= need)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    if (new_cap < need)
        new_cap = need;

    seq_t *new_cache = seq_alloc_array(new_cap);
    if (!new_cache)
        return;                           /* allocation failed: caller will see missing entries */

    if (sqfree_cache) {
        memcpy(new_cache, sqfree_cache, cache_len * sizeof(seq_t));
        seq_free_array(sqfree_cache);
    }
    sqfree_cache = new_cache;
    cache_cap    = new_cap;
}

/* Extend the cache until it contains at least 'n' square‑free numbers. */
static void build_cache(idx_t n)
{
    if (cache_len >= (size_t)n)
        return;

    while (cache_len < (size_t)n) {
        ++last_checked;
        if (is_squarefree((seq_t)last_checked)) {
            ensure_cache_capacity(cache_len + 1);
            sqfree_cache[cache_len++] = (seq_t)last_checked;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* outside the OEIS offset, defined as 0 */

    build_cache(n);
    seq_t nsq = (seq_t)n * (seq_t)n;
    return nsq + sqfree_cache[(size_t)n - 1];
}
