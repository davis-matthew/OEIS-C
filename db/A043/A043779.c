/*
 * A043779  Numbers k such that number of runs in the base 3 representation of
 *          k is congruent to 3 mod 4.
 * https://oeis.org/A043779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   count_base3_runs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043779: numbers k such that the number of runs in the
 * base‑3 representation of k is congruent to 3 mod 4.
 * This implementation generates terms sequentially, caching them
 * for efficient repeated access.  The cache is extended as needed. */

#include <stdlib.h>
#include "../utils/count_base3_runs.h"

/* cache of computed values */
static seq_t *cache = NULL;
static size_t cache_size  = 0;   /* number of stored terms */
static size_t cache_cap   = 0;   /* allocated capacity */
static seq_t next_k = (seq_t)1;  /* next integer to test */

/* grow cache capacity if needed */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * (sizeof(seq_t)));
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* n is 1‑based index */
    if (n == (idx_t)0) return (seq_t)0;  /* offset is 1; guard against 0 */

    /* generate terms up to n */
    if (cache_size < (size_t)n) {
        ensure_cache_capacity((size_t)n);
        while (cache_size < (size_t)n) {
            unsigned int runs = count_base3_runs(next_k);
            if (runs % 4 == 3) {
                cache[cache_size++] = next_k;
            }
            ++next_k;
        }
    }
    return cache[(size_t)(n - (idx_t)1)];
}
