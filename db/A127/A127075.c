/*
 * A127075  a(1)=1. a(n) = a(n-1) + (sum of the earlier terms {among terms
 *          a(1) through a(n-1)} which are coprime to n).
 * https://oeis.org/A127075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1 a(n) = a(n-1) + Σ{ a(k) | 1 ≤ k < n and gcd(a(k), n) =
 *            1 } for n ≥ 2 (This is the recurrence described in OEIS
 *            A127075.)
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 27, u64 n <= 56, u128 n <= 118,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)            /* sequence offset is 1 */
        return (seq_t)0;

    static seq_t *cache   = NULL; /* cache[0] unused, cache[1] = a(1) */
    static size_t capacity = 0;    /* allocated size (≥ last+1)          */
    static idx_t last     = 0;     /* highest index already computed    */

    /* Grow the cache if necessary */
    if (capacity < (size_t)n + 1) {
        size_t newcap = capacity ? capacity * 2 : 16;
        while (newcap < (size_t)n + 1)
            newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache = tmp;
        capacity = newcap;
    }

    /* Compute missing terms up to n */
    for (size_t i = (size_t)last + 1; i <= (size_t)n; ++i) {
        if (i == 1) {                     /* a(1) = 1 */
            cache[1] = (seq_t)1;
            continue;
        }

        /* Start with a(i-1) */
        seq_t val = cache[i - 1];

        /* Add all earlier terms that are coprime to i */
        seq_t nseq = (seq_t)i;            /* convert index to seq_t for gcd */
        for (size_t k = 1; k < i; ++k) {
            if (gcd(cache[k], nseq) == (seq_t)1)
                val += cache[k];
        }

        cache[i] = val;
    }

    last = n;
    return cache[n];
}
