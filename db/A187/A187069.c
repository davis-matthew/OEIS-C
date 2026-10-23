/*
 * A187069  Let i be in {1,2,3}, let r >= 0 be an integer and n=2*r+i-1. Then
 *          a(n)=a(2*r+i-1) gives the quantity of H_(7,2,0) tiles in a
 *          subdivided H_(7,i,r) tile after linear scaling by the factor x^r,
 *          where x=sqrt((2*cos(Pi/7))^2-1).
 * https://oeis.org/A187069
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 30, u32 n <= 58, u64 n <= 113,
 *            u128 n <= 222, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A187069
   Recurrence: a(n) = 2*a(n-2) + a(n-4) - a(n-6)
   Initial values: a(0)=0, a(1)=1, a(2)=0, a(3)=1, a(4)=1, a(5)=2
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *cache = NULL;     /* array of computed terms          */
    static size_t  cap    = 0;      /* current capacity of the cache    */
    static size_t  max_computed = 0;/* highest index computed so far    */

    /* initialise the cache on first call */
    if (cache == NULL) {
        cap = 6; /* we need space for indices 0..5 at least */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure – return 0 (should never happen) */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)1;
        cache[5] = (seq_t)2;
        max_computed = 5;
    }

    /* if the requested value is already cached, return it */
    if (n <= max_computed)
        return cache[n];

    /* ensure capacity for indices up to n */
    if (cap <= n) {
        size_t new_cap = cap;
        if (new_cap == 0) new_cap = 1;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;                 /* double until enough */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;        /* allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* compute all missing terms up to n using the recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* the recurrence uses indices i-2, i-4, i-6 which are valid
           because i >= 6 in this loop; base values for indices < 6
           were already set during initialisation. */
        cache[i] = 2 * cache[i - 2] + cache[i - 4] - cache[i - 6];
    }
    max_computed = n;
    return cache[n];
}
