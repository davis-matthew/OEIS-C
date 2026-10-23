/*
 * A043999  Numbers k such that 0 and 4 occur juxtaposed in the base-10
 *          representation of k but not of k+1.
 * https://oeis.org/A043999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   generate the smallest numbers k whose decimal representation *
 *            contains either "04" or "40" as adjacent digits, but the
 *            representation * of k+1 does not. a(1) = 40, a(2) = 104, a(3) =
 *            140, …
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has_adjacent_04_or_40
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_adjacent_04_or_40.h"

/* static cache for memoisation */
static seq_t      *cache  = NULL;   /* array of found values      */
static size_t      cache_cap = 0;   /* capacity of cache          */
static idx_t       cache_cnt  = 0;   /* number of values stored    */
static idx_t       next_to_check = 0;/* smallest integer yet untested */

/* Grow the cache array to accommodate at least `need` elements. */
static void ensure_capacity(idx_t need)
{
    if (cache_cap >= (size_t)need)
        return;
    size_t new_cap = cache_cap < 32 ? 32 : cache_cap;
    while (new_cap < (size_t)need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                    /* fatal error in memory allocation */
    cache      = tmp;
    cache_cap  = new_cap;
}

/* Generate terms until we have `target` values stored. */
static void generate_upto(idx_t target)
{
    if (target <= cache_cnt) return;
    ensure_capacity(target);
    while (cache_cnt < target) {
        seq_t k = (seq_t)next_to_check;
        if (has_adjacent_04_or_40(k) && !has_adjacent_04_or_40(k + 1))
            cache[cache_cnt++] = k;
        next_to_check++;
    }
}

/* Sequence function with external linkage. */
seq_t a(idx_t n)
{
    generate_upto(n);
    return cache[n - 1];
}
