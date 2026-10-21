/*
 * A072328  a(n+1) = 2*a(n-2) + a(n-1), with a(0) = 3, a(1) = 0, and a(2) = 2.
 * https://oeis.org/A072328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 26, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=3, a(1)=0, a(2)=2
   a(n) = 2*a(n-3) + a(n-2)   for n>=3
   The function memoises computed values, so calls may arrive in any order. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cacheSize = 0; /* number of terms currently computed (size) */
    static idx_t cacheCap  = 0; /* allocated capacity */

    /* initialise cache for the first call */
    if (cache == NULL) {
        cacheCap  = 3;          /* at least store indices 0,1,2 */
        cacheSize = 3;
        cache = (seq_t *)malloc(cacheCap * sizeof(seq_t));
        if (cache == NULL) { /* out‑of‑memory – abort (no proper error handling) */
            return (seq_t)0;
        }
        /* base values */
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)2;
    }

    /* If we need more room, grow the allocation */
    if (n >= cacheSize) {
        idx_t needed = n + 1;          /* new size after computing up to n */
        if (needed > cacheCap) {
            idx_t newCap = cacheCap > 0 ? cacheCap : 4;
            while (newCap < needed) newCap <<= 1;  /* double until enough */
            seq_t *tmp = (seq_t *)realloc(cache, newCap * sizeof(seq_t));
            if (tmp == NULL) { /* out‑of‑memory – abort */
                return (seq_t)0;
            }
            cache = tmp;
            cacheCap = newCap;
        }

        /* Compute missing terms up to n */
        for (idx_t i = cacheSize; i <= n; ++i) {
            /* recurrence: a(i) = 2*a(i-3) + a(i-2)  for i >= 3 */
            cache[i] = (seq_t)2 * cache[i - 3] + cache[i - 2];
        }
        cacheSize = n + 1;
    }

    return cache[n];
}
