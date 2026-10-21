/*
 * A202414  Number of (n+2) X 3 binary arrays with no more than one of any
 *          consecutive three bits set in any row or column.
 * https://oeis.org/A202414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + 3*a(n-3) + a(n-4) with initial values
 *            a(1)=34, a(2)=94, a(3)=265, a(4)=739
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 19, u64 n <= 40, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>


/* static cache: 1‑based indexing, cache[0] unused */
static seq_t *cache = NULL;
static size_t cache_len = 0;

/* ensure the cache has at least "needed" slots (indices 0..needed-1) */
static void grow_cache(size_t needed)
{
    if (cache_len >= needed) return;
    seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);        /* allocation failed */
    cache = tmp;
    cache_len = needed;
}

/* initialise the first four terms */
static void init_cache(void)
{
    if (cache) return;
    grow_cache(5);                         /* indices 0..4 (0 unused) */
    cache[1] = (seq_t)34;
    cache[2] = (seq_t)94;
    cache[3] = (seq_t)265;
    cache[4] = (seq_t)739;
}

/* compute a(n) for n >= 1, memoising results */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */

    init_cache();

    size_t idx = (size_t)n;
    if (idx < cache_len)                 /* already computed */
        return cache[idx];

    /* save current length before possibly reallocating */
    size_t old_len = cache_len;
    if (idx + 1 > cache_len)             /* need more space up to index n */
        grow_cache(idx + 1);

    /* start recursing from the first unfinished index */
    size_t start = old_len;
    if (start < 5) start = 5;            /* recurrence only valid from 5 */

    for (size_t i = start; i <= idx; ++i) {
        seq_t val = (seq_t)2 * cache[i - 1];
        val += cache[i - 2];
        val += (seq_t)3 * cache[i - 3];
        val += cache[i - 4];
        cache[i] = val;
    }

    return cache[idx];
}
