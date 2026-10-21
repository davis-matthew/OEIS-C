/*
 * A090041  a(n) = 10*a(n-1) - 20*a(n-2), a(0)=1, a(1)=6.
 * https://oeis.org/A090041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 10*a(n-1) - 20*a(n-2), a(0)=1, a(1)=6
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_len = 0;

    /* Extend cache capacity if needed */
    idx_t needed = n + 1;
    if (needed > cache_cap) {
        idx_t newcap = cache_cap ? cache_cap * 2 : 16;
        while (newcap < needed) newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache = tmp;
        cache_cap = newcap;
    }

    /* Ensure base values are present */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }
    if (cache_len == 1 && n >= 1) {
        cache[1] = (seq_t)6;
        cache_len = 2;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t term = ((seq_t)10) * cache[i - 1] - ((seq_t)20) * cache[i - 2];
        cache[i] = term;
    }
    cache_len = n + 1;

    return cache[n];
}
