/*
 * A021217  Decimal expansion of 1/213.
 * https://oeis.org/A021217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = nth digit of the decimal expansion of 1/213
 * (starting at the first digit after the decimal point, a(0)=0).
 *
 * Implementation: Long division with memoisation.
 * Each call computes digits up to the required index, caching the results
 * and the current remainder to avoid recomputing from scratch.
 */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* static state for memoisation */
    static seq_t *cache = NULL;          /* stored digits */
    static size_t  cache_len = 0;        /* number of digits cached (>=0) */
    static size_t  cache_cap = 0;        /* allocated capacity */
    static uint64_t rem = 1;             /* current remainder (0 <= rem < 213) */

    /* if already cached, just return */
    if (n < cache_len) {
        return cache[n];
    }

    /* extend cache to store up to n (inclusive) */
    size_t target_len = n + 1;
    if (target_len > cache_cap) {
        /* allocate more space: double or fit target */
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        if (new_cap < target_len) new_cap = target_len;
        seq_t *new_mem = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_mem) { /* allocation failed – abort */
            exit(EXIT_FAILURE);
        }
        cache = new_mem;
        cache_cap = new_cap;
    }

    /* Generate digits from current cache_len up to n */
    for (size_t i = cache_len; i < target_len; ++i) {
        rem = rem * 10ULL;              /* shift left one decimal place */
        uint64_t digit = rem / 213ULL;   /* next digit */
        rem = rem % 213ULL;             /* update remainder */
        cache[i] = (seq_t)digit;
    }
    cache_len = target_len;

    return cache[n];
}
