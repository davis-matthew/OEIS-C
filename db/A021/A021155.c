/*
 * A021155  Decimal expansion of 1/151.
 * https://oeis.org/A021155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = nth decimal digit of 1/151 * Computation via long
 *            division: digit = (remainder*10)/151. * Start with remainder =
 *            1, then repeatedly multiply by 10, * divide by 151 to produce
 *            each digit, and keep the new remainder
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Cached digits up to the current maximum index. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of digits stored */
    static size_t cache_cap = 0;   /* allocated capacity of cache */
    static uint64_t rem = 1;       /* remainder before generating next digit */

    /* Return cached digit if already computed. */
    if (n < cache_len)
        return cache[(size_t)n];

    /* Extend cache capacity if necessary. */
    size_t need = (size_t)n + 1;
    if (need > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < need)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failure – return a safe default. */
            return 0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate new digits up to index n. */
    for (size_t i = cache_len; i < need; ++i) {
        uint64_t prod = rem * 10ull;
        seq_t digit = (seq_t)(prod / 151ull);
        rem = prod % 151ull;
        cache[i] = digit;
    }

    cache_len = need;
    return cache[(size_t)n];
}
