/*
 * A039400  Numbers whose base-10 representation has the same nonzero number
 *          of 0's and 8's.
 * https://oeis.org/A039400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   equal_zero_eight
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: iterate over integers and collect numbers with equal
   nonzero counts of 0 and 8 in their decimal representation. */

/* Required headers for allocations and boolean type. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/equal_zero_eight.h"

/* Static cache for previously found terms. */
static seq_t *cache = NULL;
static size_t cache_len = 0;     /* Elements stored in cache. */
static size_t cache_cap = 0;     /* Current allocated capacity. */
static idx_t next_chk = 1;       /* Next integer to check. */

/* Ensure that the cache contains at least n terms.  The cache is
   grown lazily: we scan successive integers until enough valid
   terms are found.  The cache is thread‑unsafe; this code is
   intended for single‑threaded usage. */
static void ensure_cache(idx_t n)
{
    if ((idx_t)cache_len >= n) return;

    if (cache == NULL) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    for (; (idx_t)cache_len < n; ++next_chk) {
        seq_t val = (seq_t)next_chk;
        if (equal_zero_eight(val)) {
            if (cache_len >= cache_cap) {
                cache_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (tmp == NULL) {
                    /* Allocation failure – terminate. */
                    abort();
                }
                cache = tmp;
            }
            cache[cache_len++] = val;
        }
    }
}

/* Public entry point for the OEIS sequence.  For n < 1 the function
   returns 0, but the offset is 1 so a(1) is the first valid term. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* Undefined for zero indexing. */
    ensure_cache(n);
    return cache[n - 1];
}
