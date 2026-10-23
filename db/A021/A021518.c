/*
 * A021518  Decimal expansion of 1/514.
 * https://oeis.org/A021518
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/514) mod 10 (decimal digit of 1/514)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Static cache for previously computed digits */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    /* Remainder for the next digit; 1 corresponds to the numerator of 1/514 */
    static uint64_t rem = 1;

    if (n < cache_size) {
        return cache[(size_t)n];
    }

    /* Extend the cache up to index n */
    size_t new_size = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) {
        /* Allocation failure – return 0 (should not happen on typical inputs) */
        return 0;
    }
    cache = new_cache;

    for (size_t i = cache_size; i <= n; ++i) {
        uint64_t prod = rem * 10ULL;      /* safe: rem < 514 */
        uint64_t d   = prod / 514ULL;     /* digit */
        cache[i] = (seq_t)d;
        rem      = prod % 514ULL;         /* remainder for next digit */
    }

    cache_size = new_size;
    return cache[(size_t)n];
}
