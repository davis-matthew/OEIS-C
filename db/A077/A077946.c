/*
 * A077946  Expansion of 1/(1 - x - 2*x^2 - 2*x^3).
 * https://oeis.org/A077946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + 2*a(n-3) (a(0)=1, a(1)=1, a(2)=3)
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 27, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle small indices directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;

    /* Cache for values already computed. */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;     /* allocated size             */
    static idx_t last  = 2;     /* largest index computed */

    /* First time: allocate and pre‑compute up to a small size. */
    if (!cache) {
        cap = 8;                     /* start with room for 8 terms */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        for (idx_t i = 3; i < cap; ++i) {
            cache[i] = cache[i-1] + (seq_t)2*cache[i-2] + (seq_t)2*cache[i-3];
        }
        last = cap - 1;
    }
    else if (n > last) {            /* Need more terms */
        /* Ensure enough capacity */
        if (n + 1 > cap) {
            idx_t newcap = cap;
            while (newcap <= n) newcap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            cache = tmp;
            cap = newcap;
        }
        /* Compute the missing terms */
        for (idx_t i = last + 1; i <= n; ++i) {
            cache[i] = cache[i-1] + (seq_t)2*cache[i-2] + (seq_t)2*cache[i-3];
        }
        last = n;
    }
    /* Return the requested value */
    return cache[n];
}
