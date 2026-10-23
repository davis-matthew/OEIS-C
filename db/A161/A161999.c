/*
 * A161999  For n even a(n) = a(n-1) + 10*a(n-2), for n odd a(n) = a(n-3) + 10
 *          a(n-2); with a(1) = 0, a(2) = 1.
 * https://oeis.org/A161999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   a(n) = 20*a(n-2) - 99*a(n-4) for n ≥ 5, with initial values
 *            a(1)=0, a(2)=1, a(3)=1, a(4)=10
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t sz = 0, cap = 0;

    if ((size_t)n > sz) {
        size_t need = (size_t)n;
        if (cap < need) {
            size_t newCap = need * 2;
            seq_t *newCache = (seq_t *)malloc(newCap * sizeof(seq_t));
            if (cache) {
                memcpy(newCache, cache, sz * sizeof(seq_t));
                free(cache);
            }
            cache = newCache;
            cap   = newCap;
        }
        for (size_t i = sz; i < need; ++i) {
            if (i == 0)          cache[i] = 0;   /* a(1) */
            else if (i == 1)      cache[i] = 1;   /* a(2) */
            else if (i == 2)      cache[i] = 1;   /* a(3) */
            else if (i == 3)      cache[i] = 10;  /* a(4) */
            else                  cache[i] = 20 * cache[i-2] - 99 * cache[i-4];
        }
        sz = need;
    }
    return cache[n-1];
}
