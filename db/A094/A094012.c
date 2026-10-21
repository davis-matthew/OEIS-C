/*
 * A094012  Expansion of x*(1-6*x+10*x^2)/(1-4*x+2*x^2)^2.
 * https://oeis.org/A094012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n/4)*((3+2*sqrt(2))*(2-sqrt(2))^n +
 *            (3-2*sqrt(2))*(2+sqrt(2))^n) Recurrence: a(0)=0, a(1)=1, a(2)=2,
 *            a(3)=6, a(n)=8*a(n-1)-20*a(n-2)+16*a(n-3)-4*a(n-4) for n>=4
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache   = NULL;   /* array of computed values */
    static idx_t  cache_cap = 0;    /* current allocated size */
    static idx_t  max_comp  = 0;    /* largest index computed so far */

    /* Initialise cache with the first four terms if this is the first call */
    if (cache == NULL) {
        cache_cap = (n < 4) ? 4 : (n + 1);
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;   /* unlikely in the tests */

        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)6;
        max_comp = 3;
    }
    /* Grow the cache if the requested index is beyond the current capacity */
    else if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n)
            new_cap *= 2;               /* double until sufficient */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0;   /* unlikely in the tests */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Return the value if it is already computed */
    if (n <= max_comp)
        return cache[n];

    /* Compute missing terms using the recurrence */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        cache[i] = (seq_t)8 * cache[i-1]
                 - (seq_t)20 * cache[i-2]
                 + (seq_t)16 * cache[i-3]
                 - (seq_t)4  * cache[i-4];
    }
    max_comp = n;
    return cache[n];
}
