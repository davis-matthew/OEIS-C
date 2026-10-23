/*
 * A043387  Numbers having three 4's in base 6.
 * https://oeis.org/A043387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer whose base‑6 representation
 *            contains exactly three digits equal to 4
 * Range:     u8 n <= 1, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   count_4_in_base6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/count_4_in_base6.h"

static seq_t *cache = NULL;          /* cached terms          */
static idx_t cache_size = 0;         /* number of cached terms */
static idx_t cache_cap  = 0;         /* allocated capacity     */
static seq_t last_checked = 0;       /* last integer examined */

/* Grow the cache to hold at least 'required' terms. */
static void ensure_cache(idx_t required)
{
    if (required <= cache_size)
        return;
    /* first allocation */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    }
    /* grow capacity if necessary */
    while (cache_cap < required)
        cache_cap <<= 1;
    cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));

    /* generate terms until we reach the required count */
    while (cache_size < required) {
        ++last_checked;                          /* candidate number     */
        if (count_4_in_base6(last_checked) == 3)
            cache[cache_size++] = last_checked;  /* store valid term     */
    }
}

seq_t a(idx_t n)
{
    /* n is 1‑based; cache indices are 0‑based */
    ensure_cache(n);
    return cache[n - 1];
}
