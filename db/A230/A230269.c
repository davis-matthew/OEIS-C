/*
 * A230269  Number of n X 2 0..2 arrays x(i,j) with each element horizontally
 *          or vertically next to at least one element with value (x(i,j)+1)
 *          mod 3, and upper left element zero.
 * https://oeis.org/A230269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0, a(2)=0, a(3)=2, a(n)=4*a(n-1)-a(n-2)-2*a(n-3) for n>=4
 *            This implements a linear recurrence with memoised array
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 37, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* handle trivial case */
    if (n == 0) return 0;

    /* static cache */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* maximum n already computed */

    if (n > cache_size) {
        /* enlarge cache to size n */
        idx_t new_size = n;
        if (new_size > cache_size) {
            if (cache == NULL) {
                cache = (seq_t *)malloc(new_size * sizeof(seq_t));
            } else {
                cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
            }
            /* initialise newly added entries */
            for (idx_t i = cache_size + 1; i <= new_size; ++i) {
                if (i == 1) cache[0] = 0;
                else if (i == 2) cache[1] = 0;
                else if (i == 3) cache[2] = 2;
                else {
                    /* recurrence: a(i) = 4*a(i-1) - a(i-2) - 2*a(i-3) */
                    seq_t val = 4 * cache[i - 2] - cache[i - 3] - 2 * cache[i - 4];
                    cache[i - 1] = val;
                }
            }
            cache_size = new_size;
        }
    }
    return cache[n - 1];
}
