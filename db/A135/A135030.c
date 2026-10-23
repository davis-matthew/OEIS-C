/*
 * A135030  Generalized Fibonacci numbers: a(n) = 6*a(n-1) + 2*a(n-2).
 * https://oeis.org/A135030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0; a(1) = 1; a(n) = 6*a(n-1) + 2*a(n-2) for n ≥ 2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    if (n < 2) return (seq_t)n;   /* a(0)=0, a(1)=1 */

    idx_t needed = n + 1;
    idx_t old_cap = cache_cap;
    if (cache_cap < needed) {
        seq_t *new_cache = (seq_t*)malloc(needed * sizeof(seq_t));
        if (!new_cache) return 0; /* allocation failed */
        for (idx_t i = 0; i < old_cap; ++i) new_cache[i] = cache[i];
        free(cache);
        cache = new_cache;
        cache_cap = needed;
    }

    if (old_cap < 2) {          /* initialise missing base values */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        if (old_cap < 1) cache[1] = (seq_t)1;   /* safeguard */
    }

    for (idx_t i = (old_cap < 2 ? 2 : old_cap); i <= n; ++i) {
        cache[i] = cache[i-1] * (seq_t)6 + cache[i-2] * (seq_t)2;
    }
    return cache[n];
}
