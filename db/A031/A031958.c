/*
 * A031958  Numbers with exactly three distinct base-6 digits.
 * https://oeis.org/A031958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   has_three_distinct_base6, popcount6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence: Numbers with exactly three distinct base‑6 digits
    (OEIS A031958, offset 1)  */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/has_three_distinct_base6.h"
#include "../utils/popcount6.h"

/*  Cache for previously computed terms  */
static seq_t      *cache   = NULL;
static size_t      cache_len = 0;
static size_t      cache_cap = 0;
static idx_t       next_val  = 1;   /* next integer to test     */

static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
    if (new_cap < needed) new_cap = needed;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (cache) {
        for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
        free(cache);
    }
    cache      = new_cache;
    cache_cap  = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* defined only for n>=1   */
    while (cache_len < (size_t)n) {
        /* Find next integer with exactly three distinct base‑6 digits  */
        while (!has_three_distinct_base6(next_val)) ++next_val;
        ensure_capacity(cache_len + 1);
        cache[cache_len++] = (seq_t)next_val;
        ++next_val;
    }
    return cache[n - 1];
}
