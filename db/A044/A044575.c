/*
 * A044575  Numbers n such that string 0,7 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   brute‑force search – a(n) is the n‑th integer whose base‑8
 *            representation contains the substring “07”, while the base‑8
 *            representation of n+1 does not
 * Range:     u8 n <= 3, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   contains_07
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/contains_07.h"

/* Local cache of previously computed terms. */
static seq_t *cache = NULL;        /* array of computed terms */
static size_t cache_cap = 0;       /* allocated capacity */
static idx_t cache_len = 0;        /* number of elements stored */
static seq_t next_scan = 0;        /* next integer to test */

/* Expand the cache capacity (double it). */
static void grow_cache(void)
{
    size_t new_cap = (cache_cap == 0) ? 64 : cache_cap << 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();   /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* Ensure the cache contains at least the first n terms (1‑based index). */
static void extend_cache(idx_t n)
{
    for (; cache_len < n; ++next_scan) {
        if (contains_07(next_scan) && !contains_07(next_scan + (seq_t)1)) {
            if (cache_len == cache_cap) grow_cache();
            cache[cache_len++] = next_scan;
        }
    }
}

/* Public entry point. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* undefined for 0, return 0 for safety */
    if (cache_len < n) extend_cache(n);
    return cache[n - 1];
}
