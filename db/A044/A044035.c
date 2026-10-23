/*
 * A044035  Numbers k such that 6 and 7 occur juxtaposed in the base-10
 *          representation of k but not of k+1.
 * https://oeis.org/A044035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   contains_67_or_76
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A044035 – numbers k such that the decimal representation of k
   contains the adjacent digits 67 or 76, but the decimal representation of k+1
   does not.  */

/* The implementation scans the natural numbers in order, keeping a static
   cache of found terms so that repeated calls for arbitrary n are fast. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/contains_67_or_76.h"

/* Static state */
static seq_t *cache       = NULL;   /* dynamic array of terms          */
static idx_t cache_cap    = 0;      /* current allocation size         */
static idx_t found        = 0;      /* number of terms stored so far   */
static uint64_t last_checked = 0;   /* last integer examined           */

/* Grow the cache to hold at least n terms. */
static void grow_cache(idx_t n)
{
    if (n <= cache_cap)
        return;

    idx_t new_cap = n;
    size_t bytes = (size_t)new_cap * sizeof(seq_t);
    seq_t *new_cache = (seq_t *)malloc(bytes);
    if (new_cache == NULL)
        abort();

    if (cache != NULL && found > 0)
        memcpy(new_cache, cache, (size_t)found * sizeof(seq_t));

    free(cache);
    cache       = new_cache;
    cache_cap   = new_cap;
}

/* Generate terms until the cache holds at least n elements. */
static void generate_up_to(idx_t n)
{
    grow_cache(n);
    while (found < n) {
        ++last_checked;
        if (contains_67_or_76(last_checked) &&
            !contains_67_or_76(last_checked + 1)) {
            cache[found++] = (seq_t)last_checked;
        }
    }
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined, but harmless */
    generate_up_to(n);
    return cache[n - 1];
}
