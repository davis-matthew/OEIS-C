/*
 * A041618  Numerators of continued fraction convergents to sqrt(328).
 * https://oeis.org/A041618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(0)=18, a(1)=163, a(2)=5886, a(3)=53137; For n≥4:
 *            a(n)=326·a(n‑2)–a(n‑4)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialize cache with initial values */
    if (!cache) {
        cache_len = (n > 3 ? n + 1 : 4);
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = 18;
        cache[1] = 163;
        cache[2] = 5886;
        cache[3] = 53137;
    }

    /* grow cache if needed */
    if (n >= cache_len) {
        size_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;           /* unlikely for small n */
        cache = tmp;
        /* compute missing terms */
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = ((seq_t)326 * cache[i - 2]) - cache[i - 4];
        }
        cache_len = new_len;
    }

    return cache[n];
}
