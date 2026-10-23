/*
 * A185899  1/8 the number of n X 2 0..7 arrays with every element equal to
 *          exactly one or two of its horizontal and vertical neighbors.
 * https://oeis.org/A185899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 13*a(n-1) + 27*a(n-2) - 147*a(n-3) - 245*a(n-4) -
 *            343*a(n-5) with a(1)=1, a(2)=15, a(3)=196, a(4)=2765, a(5)=38731
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t capacity = 0;

    if (n == 0) return 0;  /* not used but harmless */

    /* initialize cache on first call */
    if (cache == NULL) {
        capacity = 5;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        /* initial terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)15;
        cache[2] = (seq_t)196;
        cache[3] = (seq_t)2765;
        cache[4] = (seq_t)38731;
    }

    /* extend cache if requested index exceeds current capacity */
    if (n > capacity) {
        idx_t new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            /* allocation failure – abort */
            exit(1);
        }
        cache = tmp;

        /* compute new terms */
        for (idx_t i = capacity + 1; i <= n; ++i) {
            /* recurrence only needed for i >= 6 */
            if (i < 6) continue;  /* initial terms already set */
            cache[i - 1] = (13 * cache[i - 2] + 27 * cache[i - 3]
                           - 147 * cache[i - 4] - 245 * cache[i - 5]
                           - 343 * cache[i - 6]);
        }
        capacity = new_cap;
    }

    return cache[n - 1];
}
