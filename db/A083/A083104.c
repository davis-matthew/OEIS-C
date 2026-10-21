/*
 * A083104  Second-order linear recurrence sequence with a(n) = a(n-1) +
 *          a(n-2), with initial terms 331635635998274737472200656430763,
 *          1510028911088401971189590305498785.
 * https://oeis.org/A083104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 n <= 27, big any n
 * Helpers:   parse_dec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A083104
   Formula: a(n) = a(n-1) + a(n-2)
   Initial values: a(0) = 331635635998274737472200656430763,
                   a(1) = 1510028911088401971189590305498785
*/

#include <stdlib.h>
#include "../utils/parse_dec.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;      /* dynamic array of computed terms */
    static idx_t cache_len = 0;      /* number of valid entries in cache */

    /* If the requested index is already cached, return it immediately */
    if (n < cache_len)
        return cache[n];

    /* Grow the cache to accommodate up to index n (inclusive) */
    idx_t new_len = n + 1;
    cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);

    /* Initialise the first two terms if this is the first call */
    if (cache_len == 0) {
        cache[0] = parse_dec("331635635998274737472200656430763");
        if (new_len > 1)
            cache[1] = parse_dec("1510028911088401971189590305498785");
    }
    /* If we already had a[0] but not a[1], initialise a[1] */
    else if (cache_len == 1) {
        cache[1] = parse_dec("1510028911088401971189590305498785");
    }

    /* Compute any missing terms using the recurrence */
    idx_t start = (cache_len < 2) ? 2 : cache_len;
    for (idx_t i = start; i < new_len; ++i) {
        cache[i] = cache[i - 1] + cache[i - 2];
    }

    cache_len = new_len;
    return cache[n];
}
