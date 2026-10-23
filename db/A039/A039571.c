/*
 * A039571  Numbers whose base-5 representation has the same number of 1's,
 *          3's and 4's.
 * https://oeis.org/A039571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th nonnegative integer whose base‑5 representation
 *            has equal numbers of 1’s, 3’s and 4’s
 * Range:     u8 n <= 28, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   has_equal_134
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_equal_134.h"

/* static state for memoisation */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;
static seq_t next_candidate = 0;

/* grow the cache array to hold at least 'needed' elements */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* public interface */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* defensive, offset is 1 */
    while (cache_len < n) {
        /* find next valid number */
        while (!has_equal_134(next_candidate))
            ++next_candidate;
        seq_t val = next_candidate;
        grow_cache((size_t)(cache_len + 1));
        cache[cache_len++] = val;
        ++next_candidate;
    }
    return cache[n - 1];
}
