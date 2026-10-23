/*
 * A117080  a(n) = 2a(n-1)+a(n-3)+1 with a(1)=1, a(2)=3, a(3)=8.
 * https://oeis.org/A117080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)+a(n-3)+1; a(1)=1, a(2)=3, a(3)=8
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, return 0 */

    static seq_t *cache = NULL;
    static size_t cached = 0;       /* number of terms stored in cache  */
    static size_t cap = 0;          /* allocated capacity in cache       */

    /* Allocate or grow cache if needed */
    if (cache == NULL) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;      /* allocation failed – should not happen */
    }
    if (cap < n) {
        size_t newcap = cap * 2;
        if (newcap < n) newcap = n;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0;      /* allocation failed – should not happen */
        cache = tmp;
        cap = newcap;
    }

    /* If requested term already computed, return it */
    if (n <= (idx_t)cached) {
        return cache[(size_t)(n - 1)];
    }

    /* Ensure the initial three terms are in the cache */
    if (cached < 1) { cache[0] = 1; cached = 1; }
    if (cached < 2) { cache[1] = 3; cached = 2; }
    if (cached < 3) { cache[2] = 8; cached = 3; }

    /* Compute missing terms sequentially */
    for (size_t i = cached; i < (size_t)n; ++i) {
        /* recurrence: a(i+1) = 2*a(i) + a(i-2) + 1 */
        cache[i] = 2 * cache[i - 1] + cache[i - 3] + 1;
    }
    cached = (size_t)n;

    return cache[(size_t)(n - 1)];
}
