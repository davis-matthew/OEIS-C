/*
 * A039526  Numbers whose base-12 representation has the same nonzero number
 *          of 3's and 7's.
 * https://oeis.org/A039526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   has_equal_3_and_7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the n‑th positive integer whose base‑12 representation
   contains the same number of non‑zero digits 3 and 7 (both counts > 0). */
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/has_equal_3_and_7.h"

/* Static cache for already computed terms */
static seq_t *cache = NULL;      /* pointer to array of seq_t */
static size_t cache_cap = 0;     /* current capacity of cache array   */
static idx_t cache_len = 0;      /* number of terms stored in cache   */
static seq_t last_checked = 0;   /* last integer examined when extending cache */

/* Ensure the cache can hold at least “need” terms */
static void grow_cache_if_needed(idx_t need)
{
    if ((size_t)need <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < (size_t)need) new_cap *= 2;

#ifdef __cplusplus
    /* Allocate a new array and copy existing elements */
    seq_t *tmp = new seq_t[new_cap];
    if (!tmp) abort();
    for (idx_t i = 0; i < cache_len; ++i) tmp[i] = cache[i];
    delete[] cache;
#else
    /* C mode: use realloc which copies existing data */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
#endif

    cache = tmp;
    cache_cap = new_cap;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1 */
    if (n <= cache_len) return cache[n - 1];

    grow_cache_if_needed(n);

    while (cache_len < n) {
        /* Find next integer with the property */
        do { ++last_checked; } while (!has_equal_3_and_7(last_checked));
        cache[cache_len++] = last_checked;
    }
    return cache[n - 1];
}
