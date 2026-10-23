/*
 * A043257  Numbers k such that 6 and 9 occur juxtaposed in the base-10
 *          representation of k but not of k-1.
 * https://oeis.org/A043257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has_adjacent_6_9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A043257:
 * Numbers k such that 6 and 9 occur juxtaposed in the base‑10 representation of k
 * but not of k−1.
 *
 * The routine generates the sequence by a simple linear search,
 * keeping an internal cache so that repeated calls are fast and
 * calls may arrive in any order.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_adjacent_6_9.h"

static seq_t *cache = NULL;        /* Cached values */
static size_t cache_count = 0;     /* Number of cached terms */
static size_t cache_cap = 0;       /* Cache allocation capacity */
static seq_t last_checked = 0;     /* Last number examined during generation */

/* Expand the cache if needed */
static void ensure_cache(size_t needed)
{
    if (cache_count >= needed) return;
    size_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort(); /* allocation failure */
    cache = new_cache;
    cache_cap = new_cap;
}

/* Generate terms until we have at least n terms cached */
static void generate_terms(idx_t n)
{
    ensure_cache((size_t)n);
    while (cache_count < (size_t)n) {
        seq_t candidate = last_checked + 1;
        ++last_checked;

        if (has_adjacent_6_9(candidate) &&
            !has_adjacent_6_9(candidate - 1)) {
            cache[cache_count++] = candidate;
        }
    }
}

/* Public entry point: a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1; behavior below offset not required */
    generate_terms(n);
    return cache[n - 1];
}
