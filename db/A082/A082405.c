/*
 * A082405  a(n) = 34*a(n-1) - a(n-2); a(0)=0, a(1)=6.
 * https://oeis.org/A082405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 34*a(n-1) - a(n-2); a(0) = 0, a(1) = 6
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for already computed values */
    static seq_t *cache = NULL;
    static idx_t cached = 0;     /* number of elements currently stored (size) */

    /* ensure cache has space for n+1 elements */
    if (cache == NULL) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failure → 0 */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)6;
        cached = 2;
    } else if (cached <= n) {
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (new_cache == NULL) return (seq_t)0;   /* allocation failure → 0 */
        cache = new_cache;
        /* initialise newly allocated slots if n >=2 */
        for (idx_t k = cached; k <= n; ++k) {
            if (k == 0)          cache[k] = (seq_t)0;
            else if (k == 1)     cache[k] = (seq_t)6;
            else                 cache[k] = ( seq_t)34 * cache[k-1] - cache[k-2];
        }
        cached = n + 1;
        return cache[n];
    }

    /* n < cached: value already computed */
    return cache[n];
}
