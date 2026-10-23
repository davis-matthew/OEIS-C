/*
 * A014118  Write in binary and read in ternary!.
 * https://oeis.org/A014118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n+1) = interpret binary representation of a(n) as a base-3
 *            number, with a(1) = 2. (This is equivalent to the OEIS
 *            recurrence a(n+1)=A005836(a(n)+1).)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 9, u64 n <= 11, u128 n <= 12,
 *            big any n
 * Helpers:   bin_to_ternary
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/bin_to_ternary.h"

/* Simple memoisation */
static seq_t *cache = NULL;      /* 1‑based indexing: cache[1] = a(1) */
static idx_t cache_cap = 0;      /* allocated capacity */
static idx_t cache_size = 0;     /* highest index computed */

static void ensure_capacity(idx_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) exit(1);   /* allocation failed */
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1; n=0 is undefined */
    ensure_capacity(n + 1);          /* +1 so we can index n safely */
    if (cache_size < 1) {            /* initialise first term */
        cache[1] = 2;
        cache_size = 1;
    }
    while (cache_size < n) {
        ++cache_size;
        cache[cache_size] = bin_to_ternary(cache[cache_size - 1]);
    }
    return cache[n];
}
