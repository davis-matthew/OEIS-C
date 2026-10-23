/*
 * A198508  Number of 2n X 2 0..4 arrays with values 0..4 introduced in row
 *          major order and each element unequal to exactly two horizontal and
 *          vertical neighbors.
 * https://oeis.org/A198508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 51*a(n-1) - 691*a(n-2) + 3601*a(n-3) - 7056*a(n-4) +
 *            4096*a(n-5) with initial values a(1)=4, a(2)=59, a(3)=1626,
 *            a(4)=52065, a(5)=1720080 (OEIS A198508)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static seq_t *cache = NULL;
static size_t cache_capacity = 0;
static idx_t cache_computed = 0;

/* Grow the cache to hold at least 'need' elements (index 0 unused) */
static void ensure_capacity(idx_t need)
{
    if ((idx_t)cache_capacity >= need)
        return;

    size_t new_cap = (size_t)need;
    seq_t *new_cache = alloc_seq_array(new_cap);
    if (new_cache == NULL)
        abort();

    /* copy existing values */
    for (idx_t i = 1; i <= cache_computed; ++i)
        new_cache[i] = cache[i];

    if (cache != NULL)
        free_seq_array(cache);

    cache = new_cache;
    cache_capacity = new_cap;
}

/* Initialise the first five terms, if not already done */
static void init_base_terms(void)
{
    if (cache_computed >= 5)
        return;

    /* need at least 6 slots (index 0 unused) */
    ensure_capacity(6);

    cache[1] = (seq_t)4;
    cache[2] = (seq_t)59;
    cache[3] = (seq_t)1626;
    cache[4] = (seq_t)52065;
    cache[5] = (seq_t)1720080;
    cache_computed = 5;
}

/* Return a(n) for n >= 1, using memoisation */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1 */

    /* ensure storage is large enough */
    ensure_capacity(n + 1);

    /* make base terms available */
    init_base_terms();

    if (n <= cache_computed)
        return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = cache_computed + 1; i <= n; ++i) {
        seq_t v1 = cache[i - 1];
        seq_t v2 = cache[i - 2];
        seq_t v3 = cache[i - 3];
        seq_t v4 = cache[i - 4];
        seq_t v5 = cache[i - 5];

        seq_t val = 51 * v1;
        val += 3601 * v3;
        val += 4096 * v5;
        val -= 691 * v2;
        val -= 7056 * v4;

        cache[i] = val;
    }
    cache_computed = n;
    return cache[n];
}
