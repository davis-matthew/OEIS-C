/*
 * A084085  Length of lists created by n substitutions k ->
 *          Range[0,Mod[k+1,4]] starting with {0}.
 * https://oeis.org/A084085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A084085
   Length of lists obtained after n substitutions k -> Range[0, Mod[k+1,4]]
   starting from {0}.
   Recurrence (for n >= 4):
       a(0)=1, a(1)=2, a(2)=5, a(3)=14
       a(n) = 3*a(n-1) - a(n-2) + a(n-4)
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* capacity of the allocated array */
    static idx_t cache_next = 0;  /* first index that has not been computed yet */

    /* first invocation: allocate cache big enough for at least the first 4 terms */
    if (cache == NULL) {
        cache_cap = (n + 1) > 4 ? (n + 1) : 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        /* initialise the first four base values */
        cache[0] = 1;
        cache[1] = 2;
        cache[2] = 5;
        cache[3] = 14;
        cache_next = 4; /* next index to compute */
    }

    /* extend the cache if necessary */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_next; i <= n; ++i) {
        /* recurrence valid only for i >= 4, but we guard with i>=4 anyway */
        seq_t val = cache[i-1] * 3;
        val -= cache[i-2];
        val += cache[i-4];
        cache[i] = val;
    }
    if (n >= cache_next)
        cache_next = n + 1;

    return cache[n];
}
