/*
 * A226592  a(n) = 2*( a(n-5) + a(n-8) + a(n-11) ) for n >= 12.
 * https://oeis.org/A226592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 46, u32 n <= 92, u64 n <= 182,
 *            u128 n <= 362, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A226592
   a(0) … a(4)   = 1
   a(5) … a(11)  = a(n-3) + 2*a(n-5)
   a(n) for n>=12 = 2*( a(n-5) + a(n-8) + a(n-11) )
   (recurrence and initial values as in OEIS entry) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: index i stores a(i) for 0 <= i < cap      */
    static seq_t *cache = NULL;
    static size_t   cap   = 0;   /* allocated size (number of seq_t elements) */
    static size_t   calc  = 0;   /* next index that needs a value          */

    size_t target = (size_t)n;   /* convert index to size_t for array ops   */

    /* Grow cache array if necessary: need to hold indices 0 … target */
    if (target + 1 > cap) {
        size_t newcap = cap ? cap * 2 : 64;
        while (newcap <= target + 1)
            newcap <<= 1;                 /* double until large enough   */
        void *tmp = realloc(cache, newcap * sizeof(*cache));
        if (!tmp)
            abort();                     /* allocation failure          */
        cache = (seq_t *)tmp;
        cap   = newcap;
    }

    /* Compute all missing terms up to target */
    for (size_t i = calc; i <= target; ++i) {
        if (i <= 4) {
            cache[i] = (seq_t)1;
        } else if (i <= 11) {
            cache[i] = cache[i-3] + (seq_t)2 * cache[i-5];
        } else {
            cache[i] = (seq_t)2 *
                       (cache[i-5] + cache[i-8] + cache[i-11]);
        }
    }
    calc = target + 1;   /* all indices < calc are now initialized */

    return cache[target];
}
