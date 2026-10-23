/*
 * A039065  Numbers whose base-8 representation has the same number of 2's and
 *          6's.
 * https://oeis.org/A039065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   equal23bit6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A039065
 * Numbers whose base‑8 representation has the same number of 2's and 6's.
 *
 * An efficient way is to generate the sequence by scanning integers
 * in ascending order and collecting those that satisfy the digit
 * property.  The search is memoised so that repeated calls with any
 * index order are handled in O(1) after the initial build.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/equal23bit6.h"

static seq_t *cache = NULL;               /* array of found terms   */
static idx_t  cache_size = 0;             /* number of cached terms */
static idx_t  cache_cap   = 0;             /* allocated capacity    */
static seq_t  next_candidate = 0;         /* next number to test   */

/* Grow the cache array to accommodate at least 'need' elements. */
static void grow_cache(idx_t need) {
    if (need <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort(); /* memory allocation failed */
    cache = new_cache;
    cache_cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for n < 1, guard */
    /* Extend cache until it contains at least n terms. */
    while (cache_size < n) {
        if (equal23bit6(next_candidate)) {
            grow_cache(cache_size + 1);
            cache[cache_size++] = next_candidate;
        }
        next_candidate++;
    }
    return cache[n - 1];
}
