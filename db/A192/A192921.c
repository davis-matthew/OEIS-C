/*
 * A192921  Constant term in the reduction by (x^2->x+1) of the polynomial
 *          p(n,x) defined below at Comments.
 * https://oeis.org/A192921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, a(2)=2, a(n)=2*a(n-1)+2*a(n-2)-a(n-3) for n>=3
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache     = NULL;   /* array of computed a(i)   */
    static idx_t  capacity  = 0;      /* allocated size (>= 3)    */
    static idx_t  computed  = 0;      /* last index computed      */

    /* initialise cache on first call */
    if (capacity == 0) {
        capacity = 3;                     /* need slots 0,1,2 */
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)2;
        computed = 2;                     /* indices 0..2 computed   */
    }

    /* if requested n is beyond current capacity, resize */
    if (n >= capacity) {
        capacity = n + 1;                  /* minimal new size     */
        cache = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
    }

    /* compute up to n if necessary */
    if (n > computed) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            /* recurrence: a(i) = 2*a(i-1) + 2*a(i-2) - a(i-3) */
            cache[i] = (seq_t)(
                ( (seq_t)2 * cache[i - 1]
                + (seq_t)2 * cache[i - 2]
                - cache[i - 3] ) );
        }
        computed = n;
    }

    return cache[n];
}
