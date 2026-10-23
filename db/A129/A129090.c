/*
 * A129090  a(1)=1; a(n) = gcd(a(n-1), n) + lcm(a(n-1), n).
 * https://oeis.org/A129090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(a(n-1), n) + lcm(a(n-1), n)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 24, u128 n <= 39,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>      /* size_t */
#include <stdlib.h>     /* malloc, free */
#include "../utils/gcd.h"  /* seq_t gcd(seq_t a, seq_t b) */

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap = 0;      /* capacity of cache array (indices 0 .. cap-1) */
    static size_t computed = 0; /* largest index whose value is known */

    /* initialise cache on first call */
    if (cap == 0) {
        cap = 2;                 /* enough for a(1) */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = 0;            /* unused */
        cache[1] = 1;            /* a(1) = 1 */
        computed = 1;
    }

    /* grow the cache if required */
    if ((size_t)n >= cap) {
        size_t newcap = cap * 2;
        if (newcap <= (size_t)n)
            newcap = (size_t)n + 1;
        seq_t *newcache = (seq_t *)malloc(newcap * sizeof(seq_t));
        for (size_t i = 0; i < cap; ++i)
            newcache[i] = cache[i];
        free(cache);
        cache = newcache;
        cap = newcap;
    }

    /* compute successive terms up to n */
    if ((size_t)n > computed) {
        for (idx_t i = (idx_t)(computed + 1); (size_t)i <= n; ++i) {
            seq_t prev = cache[i - 1];
            seq_t g = gcd(prev, (seq_t)i);
            seq_t l = (prev / g) * (seq_t)i;   /* lcm = (prev/g)*i */
            cache[i] = g + l;
        }
        computed = (size_t)n;
    }

    return cache[n];
}
