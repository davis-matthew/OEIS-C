/*
 * A236392  Expansion of g.f. (x+x^3+x^4)/(1-x-x^3-2*x^4).
 * https://oeis.org/A236392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3) + 2*a(n-4), a(0) = 0, a(1) = 1, a(2) = 1,
 *            a(3) = 2, a(4) = 4
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 42, u64 n <= 82, u128 n <= 164,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;           /* number of elements stored */

    /* Ensure there is enough space for indices up to n */
    size_t needed = (size_t)(n + 1);
    if (cache == NULL) {
        cache = (seq_t*)malloc(sizeof(seq_t) * needed);
        cache_len = 0;
    } else if (needed > cache_len) {
        cache = (seq_t*)realloc(cache, sizeof(seq_t) * needed);
    }

    /* Initialise the base values */
    if (cache_len == 0) {
        cache[0] = (seq_t)0;
        cache_len = 1;
    }
    while (cache_len <= 4 && cache_len <= n) {
        switch (cache_len) {
            case 1: cache[1] = (seq_t)1; break;
            case 2: cache[2] = (seq_t)1; break;
            case 3: cache[3] = (seq_t)2; break;
            case 4: cache[4] = (seq_t)4; break;
        }
        cache_len++;
    }

    /* Compute all missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = cache[i-1] + cache[i-3] + cache[i-4] * (seq_t)2;
    }
    cache_len = (idx_t)(n + 1);

    return cache[n];
}
