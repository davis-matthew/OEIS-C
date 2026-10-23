/*
 * A174776  y-values in the solution to x^2 - 39*y^2 = 1.
 * https://oeis.org/A174776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 50*a(n-1)-a(n-2), a(1)=0, a(2)=4
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_cap = 0;

    if (n < 1)
        return 0;                   /* undefined for n<1, return 0 for safety */

    if (n > cache_size) {           /* need to extend cache */
        idx_t needed = n;
        if (needed > cache_cap) {    /* grow capacity */
            idx_t new_cap = needed * 2;
            if (new_cap < needed) new_cap = needed; /* safety against overflow */
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }
        for (idx_t i = cache_size; i < n; ++i) {
            if (i == 0) {
                cache[0] = 0;       /* a(1) */
            } else if (i == 1) {
                cache[1] = 4;       /* a(2) */
            } else {
                cache[i] = (seq_t)50 * cache[i-1] - cache[i-2];
            }
        }
        cache_size = n;
    }
    return cache[n-1];               /* array index is (n-1) */
}
