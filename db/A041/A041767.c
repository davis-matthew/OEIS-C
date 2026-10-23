/*
 * A041767  Denominators of continued fraction convergents to sqrt(404).
 * https://oeis.org/A041767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 402*a(n-2) - a(n-4), a(0)=1, a(1)=10, a(2)=401, a(3)=4020
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;  /* number of initialized entries */

    /* Grow cache to accommodate index n */
    size_t needed = (size_t)n + 1;
    if (cache == NULL) {
        cache = (seq_t*)malloc(needed * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* allocation failed */
    } else if (cache_len < needed) {
        seq_t *tmp = (seq_t*)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
    }

    /* Initialize missing base values 0..3 */
    for (idx_t i = cache_len; i < 4 && i <= n; ++i) {
        if (i == 0) cache[0] = (seq_t)1;
        else if (i == 1) cache[1] = (seq_t)10;
        else if (i == 2) cache[2] = (seq_t)401;
        else if (i == 3) cache[3] = (seq_t)4020;
    }

    /* Compute the sequence from the first missing index up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 4) continue;          /* already set */
        cache[i] = (seq_t)402 * cache[i-2] - cache[i-4];
    }

    cache_len = needed;
    return cache[n];
}
