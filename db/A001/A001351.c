/*
 * A001351  Associated Mersenne numbers.
 * https://oeis.org/A001351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) - a(n-2) + 3*a(n-3) - a(n-4) + a(n-5) - a(n-6) for
 *            n >= 6, with a(0) = 0, a(1) = 1, a(2) = 3, a(3) = 1, a(4) = 3,
 *            a(5) = 11
 * Range:     u8 n <= 14, u16 n <= 28, u32 n <= 58, u64 n <= 116,
 *            u128 n <= 232, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    
    static seq_t *cache = NULL;
    static idx_t cache_capacity = 0;
    static idx_t computed_up_to = 5;

    if (cache != NULL && n <= computed_up_to) {
        return cache[n];
    }

    if (cache == NULL) {
        cache_capacity = (n + 1 > 256) ? n + 1 : 256;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 3;
        cache[3] = 1;
        cache[4] = 3;
        cache[5] = 11;
        computed_up_to = 5;
    }

    if (n <= computed_up_to) {
        return cache[n];
    }

    if (n >= cache_capacity) {
        idx_t new_capacity = cache_capacity;
        while (new_capacity <= n) new_capacity *= 2;
        cache = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
        cache_capacity = new_capacity;
    }

    for (idx_t i = computed_up_to + 1; i <= n; i++) {
        cache[i] = cache[i-1] - cache[i-2] + 3*cache[i-3] - cache[i-4] + cache[i-5] - cache[i-6];
    }
    computed_up_to = n;

    return cache[n];
}
