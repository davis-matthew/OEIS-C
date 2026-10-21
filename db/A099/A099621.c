/*
 * A099621  a(n) = Sum_{k=0..floor(n/2)} binomial(n-k,k+1) *
 *          3^(n-k-1)*(4/3)^k.
 * https://oeis.org/A099621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (4^(n+2) - 5*3^(n+1) - (-1)^n)/20 (recurrence
 *            a(n)=6*a(n-1)-5*a(n-2)-12*a(n-3), a(0)=0, a(1)=1, a(2)=6)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;          /* array of computed values   */
    static idx_t cache_capacity = 0;     /* allocated array size       */
    static idx_t cache_size = 0;         /* number of values computed (next free index) */

    /* ensure capacity for index n */
    if (n >= cache_capacity) {
        idx_t new_cap = cache_capacity ? cache_capacity << 1 : 8;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();               /* out of memory: abort */
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* initialise base cases only once */
    if (cache_size == 0) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)6;
        cache_size = 3;
    }

    /* if the requested value is already computed, return it */
    if (n < cache_size) return cache[n];

    /* compute missing values up to n using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t t = 6 * cache[i-1];
        t -= 5 * cache[i-2];
        t -= 12 * cache[i-3];
        cache[i] = t;
    }
    cache_size = n + 1;
    return cache[n];
}
