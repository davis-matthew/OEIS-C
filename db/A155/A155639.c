/*
 * A155639  a(n) = 6^n-5^n+1^n.
 * https://oeis.org/A155639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^n - 5^n + 1^n * Recurrence: a(0) = 1, a(1) = 2, * a(n)
 *            = 11*a(n-1) - 30*a(n-2) + 20 (n ≥ 2) * The recurrence is
 *            rearranged to avoid signed intermediate values: * a(n) =
 *            (11*a(n-1) + 20) - 30*a(n-2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>   /* for memset */

/* static cache for memoised values */
static seq_t *cache = NULL;
static idx_t   cache_len = 0;

/* Ensure the cache has space for indices up to n (inclusive). */
static void
ensure_cache(idx_t n)
{
    /* No reallocation required if capacity is sufficient. */
    if (cache_len > n)
        return;

    /* Grow the cache: new size at least twice the required size. */
    size_t required = (size_t)(n + 1);
    size_t new_len = required * 2;
    if (new_len < required)          /* overflow protection */
        new_len = required;

    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) {                       /* allocation failure */
        abort();
    }

    /* Zero any newly allocated elements so we can test for 0 (uncomputed). */
    if (new_len > cache_len) {
        memset(tmp + cache_len, 0, (new_len - cache_len) * sizeof(seq_t));
    }

    cache      = tmp;
    cache_len  = new_len;
}

/* Public sequence function. */
seq_t
a(idx_t n)
{
    /* First, make sure we have enough space. */
    ensure_cache(n);

    /* Compute missing values up to n, using memoisation of already
       computed entries.  Zero is used as a sentinel for 'not yet
       computed', which is safe because all sequence values are positive. */
    for (size_t i = 0; i <= (size_t)n; ++i) {
        if (cache[i] != 0)          /* already computed */
            continue;

        if (i == 0)
            cache[i] = 1;
        else if (i == 1)
            cache[i] = 2;
        else {
            /* Recurrence formulated to avoid unsigned underflow:
               a(n) = (11*a(n-1) + 20) - 30*a(n-2)  */
            cache[i] =
              ((seq_t)11 * cache[i - 1] + 20) - ((seq_t)30 * cache[i - 2]);
        }
    }

    return cache[(size_t)n];
}
