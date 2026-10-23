/*
 * A192237  a(n) = 2*(a(n-1) + a(n-2) + a(n-3)) - a(n-4) for n >= 4, with
 *          initial terms 0,0,0,1.
 * https://oeis.org/A192237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*(a(n-1)+a(n-2)+a(n-3)) - a(n-4) initial values: a(0)=0,
 *            a(1)=0, a(2)=0, a(3)=1 Computes the OEIS sequence A192237
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 24, u64 n <= 45, u128 n <= 86,
 *            big any n
 * Helpers:   seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_alloc_array.h"   /* seq_t* seq_alloc_array(size_t n); */

seq_t a(idx_t n)
{
    /* memoization cache */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;         /* allocated length of cache   */
    static size_t  last  = 0;         /* highest index already computed */

    /* ensure the cache is large enough */
    if ((size_t)n + 1 > cap) {
        size_t newcap = cap ? cap * 2 : 4;            /* start with 4 for the base case */
        while (newcap <= (size_t)n + 1) newcap <<= 1;/* power‑of‑two growth */

        seq_t *tmp = seq_alloc_array(newcap);
        if (cache) {
            /* copy existing values */
            for (size_t i = 0; i < cap; ++i) tmp[i] = cache[i];
            free(cache);
        }
        cache = tmp;
        cap   = newcap;
    }

    /* initialise the first four terms once */
    if (last < 3) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)1;
        last     = 3;
    }

    /* compute missing terms up to n */
    for (size_t k = last + 1; k <= (size_t)n; ++k) {
        seq_t s = cache[k-1] + cache[k-2] + cache[k-3];
        s = s * (seq_t)2;
        cache[k] = s - cache[k-4];
    }
    if ((size_t)n > last) last = (size_t)n;

    return cache[(size_t)n];
}
