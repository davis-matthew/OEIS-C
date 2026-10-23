/*
 * A015555  Expansion of x/(1 - 7*x - 2*x^2).
 * https://oeis.org/A015555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) + 2*a(n-2); a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* base cases */
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* first time: initialise cache up to n */
    if (cache == NULL) {
        cache_size = n + 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        cache[0] = 0;
        cache[1] = 1;
        for (idx_t i = 2; i <= n; ++i) {
            cache[i] = (seq_t)7 * cache[i-1] + (seq_t)2 * cache[i-2];
        }
        return cache[n];
    }

    /* enlarge cache if needed and compute new entries */
    if (cache_size <= n) {
        idx_t old_size = cache_size;
        cache_size = n + 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_size);
        for (idx_t i = old_size; i <= n; ++i) {
            cache[i] = (seq_t)7 * cache[i-1] + (seq_t)2 * cache[i-2];
        }
    }

    return cache[n];
}
