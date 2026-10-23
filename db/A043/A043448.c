/*
 * A043448  Numbers having four 6's in base 8.
 * https://oeis.org/A043448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 31, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   has_four_six_base8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers having exactly four 6's in base 8 (search) */
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/has_four_six_base8.h"

/* cache for already found numbers */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_size = 0;
static seq_t last_candidate = 0;

/* grow the cache array if more space is required */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp && new_cap > 0)
        abort();                      /* allocation failed */
    cache        = tmp;
    cache_cap    = new_cap;
}

/* return the n‑th number (n ≥ 1) having exactly four 6's in base 8 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined for offset 0 */

    /* find enough candidates */
    while (cache_size < (size_t)n)
    {
        ++last_candidate;
        if (has_four_six_base8(last_candidate))
        {
            ensure_cache_capacity(cache_size + 1);
            cache[cache_size++] = last_candidate;
        }
    }
    return cache[(size_t)n - 1];
}
