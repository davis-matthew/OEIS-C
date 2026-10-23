/*
 * A039365  Numbers whose base-9 representation has the same nonzero number of
 *          1's and 2's.
 * https://oeis.org/A039365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   numbers whose base-9 representation has the same nonzero number
 *            of 1's and 2's
 * Range:     u8 n <= 31, u16 n <= 8513, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_equal_nonzero_12
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/has_equal_nonzero_12.h"

/* Cache for previously found terms */
static seq_t *seq_cache        = NULL; /* array of previously computed terms */
static size_t seq_cache_len   = 0;     /* how many terms are stored */
static size_t seq_cache_cap   = 0;     /* allocated capacity */

/* Grow the cache if it cannot hold 'needed' elements */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= seq_cache_cap)
        return;
    size_t new_cap = seq_cache_cap ? seq_cache_cap * 2 : 8;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cache = tmp;
    seq_cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */

    if ((size_t)n <= seq_cache_len)      /* already cached */
        return seq_cache[(size_t)n - 1];

    /* start searching from last cached value + 1 or from 1 */
    seq_t cur = (seq_cache_len > 0) ? (seq_cache[seq_cache_len - 1] + 1) : 1;

    while (seq_cache_len < (size_t)n) {
        if (has_equal_nonzero_12(cur)) {
            ensure_cache_capacity(seq_cache_len + 1);
            seq_cache[seq_cache_len] = cur;
            ++seq_cache_len;
        }
        ++cur;
    }
    return seq_cache[(size_t)n - 1];
}
