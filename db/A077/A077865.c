/*
 * A077865  Expansion of (1-x)^(-1)/(1-x-2*x^2+x^3).
 * https://oeis.org/A077865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A077865: Expansion of (1-x)^(-1)/(1-x-2*x^2+x^3). 
   Recurrence:  a(0)=1, a(1)=2, a(2)=5,
                 a(n)=a(n-1)+2*a(n-2)-a(n-3)+1   (n>=3)
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;      /* array holding a(k) for k < cache_len */
    static idx_t cache_len = 0;      /* number of entries currently stored */

    /* if requested value already computed, return it */
    if (n < cache_len)
        return cache[(size_t)n];

    /* enlarge cache to hold up to n */
    size_t new_len = (size_t)n + 1;
    cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!cache)
        return (seq_t)0;   /* out‑of‑memory; unlikely for the intended sizes */

    /* initialise and compute missing terms */
    for (size_t i = cache_len; i <= new_len - 1; ++i) {
        if (i == 0)                     cache[i] = (seq_t)1;
        else if (i == 1)                cache[i] = (seq_t)2;
        else if (i == 2)                cache[i] = (seq_t)5;
        else /* i >= 3 */ {
            /* a(i) = a(i-1) + 2*a(i-2) - a(i-3) + 1
               operations are performed modulo the natural width of seq_t
               which is correct for all values that fit in seq_t          */
            cache[i] = cache[i-1] + (seq_t)2 * cache[i-2] - cache[i-3] + (seq_t)1;
        }
    }
    cache_len = new_len;
    return cache[(size_t)n];
}
