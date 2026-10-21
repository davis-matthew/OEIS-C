/*
 * A210127  Number of (n+1) X 2 0..3 arrays with every 2 X 2 subblock having
 *          one, three or four distinct values, and new values 0..3 introduced
 *          in row major order.
 * https://oeis.org/A210127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*a(n-1) - 9*a(n-2) - 2*a(n-3) with initial values
 *            a(1)=8, a(2)=81, a(3)=897 (1-based indexing, a(1) is the first
 *            term)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    static size_t cache_len = 0;   /* highest index computed */

    /* Handle the first three terms directly */
    if (n == 1) return (seq_t)8;
    if (n == 2) return (seq_t)81;
    if (n == 3) return (seq_t)897;

    /* If the requested index is already cached, return it */
    if (n <= cache_len) {
        return cache[(size_t)n];
    }

    /* Grow the cache to accommodate index n */
    grow_seq(&cache, &cache_cap, (size_t)n + 1);

    /* Initialise the base values if not already done */
    if (cache_len < 3) {
        cache[1] = (seq_t)8;
        cache[2] = (seq_t)81;
        cache[3] = (seq_t)897;
        cache_len = 3;
    }

    /* Compute missing terms up to n */
    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        cache[i] = ((seq_t)12 * a1) - ((seq_t)9 * a2) - ((seq_t)2 * a3);
    }

    cache_len = (size_t)n;
    return cache[(size_t)n];
}
