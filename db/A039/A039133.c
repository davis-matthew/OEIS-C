/*
 * A039133  Numbers whose base-10 representation has the same number of 2's
 *          and 8's.
 * https://oeis.org/A039133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth non‑negative integer whose decimal *
 *            representation contains the same number of 2's as 8's *
 *            (enumerated starting from a(1)=0)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   digit_frequencies
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/digit_frequencies.h"

seq_t a(idx_t n) {
    /* Static cache of found terms and state for incremental generation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms currently cached */
    static size_t cache_cap = 0;   /* current capacity of the cache   */
    static seq_t next_candidate = 0; /* next integer to test */

    /* Initialise the cache on the first call */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return (seq_t)0;          /* out of memory; unlikely */
    }

    /* Expand the cache size when needed */
    if (cache_len >= cache_cap) {
        cache_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
        if (!tmp) return (seq_t)0;            /* out of memory; unlikely */
        cache = tmp;
    }

    /* Generate terms until we have at least n of them cached */
    for (size_t target = (size_t)n; cache_len < target; ++next_candidate) {
        unsigned int freq[10] = {0};
        digit_frequencies(next_candidate, freq);
        if (freq[2] == freq[8]) {
            cache[cache_len++] = next_candidate;
        }
    }

    return cache[n - 1];
}
