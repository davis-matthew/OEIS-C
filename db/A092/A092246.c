/*
 * A092246  Odd "odious" numbers (A000069).
 * https://oeis.org/A092246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = odd odious numbers. Computed by iterating over odd
 *            numbers and selecting those with an odd binary popcount
 * Range:     u8 n <= 64, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   popcount_parity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/popcount_parity.h"

/* static cache of computed terms */
static seq_t *cache     = NULL;  /* array of terms */
static size_t cache_len = 0;     /* number of terms stored */
static size_t cache_cap = 0;     /* allocated capacity of the array */

/* next odd number to test */
static seq_t next_candidate = 1;

/* grow the cache array to at least new_len elements */
static void cache_grow(size_t new_len)
{
    if (new_len <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < new_len)
        new_cap *= 2;
    cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* sequence term a(n), 1‑based index */
seq_t a(idx_t n)
{
    if (n == 0)    /* offset starts at 1, undefined for 0 – return 0 */
        return 0;

    /* extend cache until we have at least n terms */
    if (cache_len < (size_t)n) {
        while (cache_len < (size_t)n) {
            if (popcount_parity(next_candidate)) {
                cache_grow(cache_len + 1);
                cache[cache_len++] = next_candidate;
            }
            next_candidate += 2;  /* next odd number */
        }
    }
    return cache[n - 1];
}
