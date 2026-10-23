/*
 * A028137  Expansion of 1/((1-4x)(1-6x)(1-8x)(1-11x)).
 * https://oeis.org/A028137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(0)=1, a(1)=29, a(2)=539, a(3)=8209,
   a(n)=29*a(n-1)-302*a(n-2)+1336*a(n-3)-2112*a(n-4)  for n≥4
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t have = 0;   /* number of elements currently stored */
    static idx_t cap   = 0;   /* allocated capacity */

    /* initialise cache with first four terms */
    if (cap == 0) {
        cap = 8;                     /* initial capacity */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)29;
        cache[2] = (seq_t)539;
        cache[3] = (seq_t)8209;
        have = 4;
    }

    /* if requested term already computed, return */
    if (n < have)
        return cache[n];

    /* grow the cache if necessary */
    if (n + 1 > cap) {
        size_t new_cap = cap;
        while ((size_t)(new_cap) < (size_t)(n + 1))
            new_cap = new_cap * 2 + 16;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = have; i <= n; ++i) {
        /* recurrence: a(i) = 29*a(i-1) - 302*a(i-2) + 1336*a(i-3) - 2112*a(i-4) */
        seq_t val = cache[i - 1] * (seq_t)29
                 - cache[i - 2] * (seq_t)302
                 + cache[i - 3] * (seq_t)1336
                 - cache[i - 4] * (seq_t)2112;
        cache[i] = val;
    }
    have = n + 1;

    return cache[n];
}
