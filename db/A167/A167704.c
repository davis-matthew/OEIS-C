/*
 * A167704  a(0)=1, a(1)=0, a(2)=2, a(3)=1, a(n)=a(n-2)+a(n-3)+a(n-4) for n>3.
 * https://oeis.org/A167704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 30, u32 n <= 59, u64 n <= 117,
 *            u128 n <= 233, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A167704
   Recurrence: a(0)=1, a(1)=0, a(2)=2, a(3)=1,
               a(n)=a(n-2)+a(n-3)+a(n-4)  for n>3
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *cache      = NULL; /* pointer to sequence values */
    static size_t cache_cap  = 0;    /* current allocated capacity */
    static idx_t  computed   = 0;    /* number of elements computed (up to index computed-1) */

    /* ensure the cache has capacity for indices up to n */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        if (new_cap < 8) new_cap = 8;          /* start with a modest size */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute values up to n if not already done */
    for (idx_t i = computed; i <= n; ++i) {
        if (i == 0)
            cache[i] = (seq_t)1;
        else if (i == 1)
            cache[i] = (seq_t)0;
        else if (i == 2)
            cache[i] = (seq_t)2;
        else if (i == 3)
            cache[i] = (seq_t)1;
        else
            cache[i] = cache[i-2] + cache[i-3] + cache[i-4];
    }
    computed = n + 1;   /* now we have values up to n */

    return cache[n];
}
