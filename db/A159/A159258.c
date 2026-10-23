/*
 * A159258  a(0)=155; for n > 0, a(n) = a(n-1) + floor(sqrt(a(n-1))).
 * https://oeis.org/A159258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + floor_sqrt(a(n-1)), a(0) = 155
 * Range:     u8 n <= 0, u16 n <= 49, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    const size_t target = (size_t)n;

    /* initialise cache for a(0) */
    if (cap == 0) {
        cap = 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)155;
        len = 1;
    }

    /* grow the array if necessary */
    if (target >= cap) {
        size_t newcap = cap;
        while (target >= newcap) newcap = newcap ? newcap << 1 : 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache = tmp;
        cap = newcap;
    }

    /* compute missing terms up to n */
    if (target >= len) {
        for (size_t i = len; i <= target; ++i) {
            seq_t prev = cache[i - 1];
            seq_t inc  = floor_sqrt(prev);
            cache[i]   = prev + inc;
        }
        len = target + 1;
    }

    return cache[target];
}
