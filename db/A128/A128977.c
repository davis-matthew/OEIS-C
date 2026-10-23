/*
 * A128977  a(0)=a(1)=1; a(n) = lcm(a(n-1) + a(n-2), n).
 * https://oeis.org/A128977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=a(1)=1; a(n)=lcm(a(n-1)+a(n-2), n) */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Base cases a(0)=a(1)=1 */
    if (n == 0 || n == 1)
        return (seq_t)1;

    static seq_t *cache = NULL;
    static idx_t cache_last = 0;   /* highest index computed */
    static size_t cache_cap = 0;   /* allocated size */

    /* Lazy initialize cache */
    if (!cache) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = cache[1] = (seq_t)1;
        cache_last = 1;
    }

    /* Grow cache to accommodate index n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n (any call order) */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        seq_t sum = cache[i - 1] + cache[i - 2];
        seq_t g   = gcd(sum, (seq_t)i);            /* lcm(x, i) = (x/gcd(x,i)) * i */
        cache[i] = (sum / g) * (seq_t)i;
    }
    cache_last = n;
    return cache[n];
}
