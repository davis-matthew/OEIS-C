/*
 * A044691  Numbers n such that string 6,5 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   contains_65
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n such that the base‑9 representation of n contains the
   substring "65" but the representation of n+1 does not. This implementation
   searches sequentially and memoises the found values. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/contains_65.h"

/* Does the integer cand satisfy the OEIS condition? */
static bool is_candidate(idx_t cand)
{
    seq_t n = (seq_t)cand;
    return contains_65(n) && !contains_65(n + 1);
}

/* Cache for memoised sequence values */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;
static idx_t  next_cand  = 1;          /* next integer to test */

/* Ensure the cache has room for another element. */
static void ensure_cache_capacity(void)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_arr) exit(EXIT_FAILURE);
        if (cache && cache_len > 0)
            memcpy(new_arr, cache, cache_len * sizeof(seq_t));
        free(cache);
        cache = new_arr;
        cache_cap = new_cap;
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* offset is 1 */
    while (cache_len < (size_t)n) {
        if (is_candidate(next_cand)) {
            ensure_cache_capacity();
            cache[cache_len++] = (seq_t)next_cand;
        }
        ++next_cand;
    }
    return cache[n - 1];
}
