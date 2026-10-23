/*
 * A127226  a(n) = 2*a(n-1) + 6*a(n-2) with a(0)=2, a(1)=2.
 * https://oeis.org/A127226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + 6*a(n-2) (a(0)=2, a(1)=2)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache[0..last_filled] holds a[i] already */
static size_t cap = 0;        /* current allocated cache size */
static idx_t last_filled = 0; /* highest n for which cache is valid */

static void grow_cache(size_t needed)
{
    if (cap >= needed)
        return;
    size_t newcap = cap ? cap * 2 : 16;
    while (newcap < needed)
        newcap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp) {
        /* In a real program we should handle allocation failure. */
        return;
    }
    cache = tmp;
    cap = newcap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)2;

    if (cache == NULL) {
        grow_cache((size_t)n + 1);
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)2;
        last_filled = 1;
    } else {
        if (n >= cap)
            grow_cache((size_t)n + 1);
        if (last_filled < 1) {
            cache[0] = (seq_t)2;
            cache[1] = (seq_t)2;
            last_filled = 1;
        }
    }

    if (n <= last_filled)
        return cache[n];

    for (idx_t i = last_filled + 1; i <= n; ++i) {
        cache[i] = cache[i - 1] * (seq_t)2 + cache[i - 2] * (seq_t)6;
    }
    last_filled = n;
    return cache[n];
}
