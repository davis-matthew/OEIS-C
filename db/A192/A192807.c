/*
 * A192807  Coefficient of x in the reduction of the polynomial (x^2 + x +
 *          1)^n by x^3 -> x^2 + x + 1.
 * https://oeis.org/A192807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 5*a(n-2) + a(n-3) with initial values: a(0)=0,
 *            a(1)=1, a(2)=6
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

extern seq_t a(idx_t n) {
    static seq_t *cache = NULL;  /* memoisation array */
    static idx_t max_n   = 0;    /* highest n already computed */

    /* Initialise cache on first call */
    if (cache == NULL) {
        /* Allocate at least space for indices 0..2 */
        size_t init_cap = (n + 1 > 3 ? n + 1 : 3);
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        /* Base values */
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 6;
        max_n = 2; /* we have computed up to index 2 */
    }

    /* Grow cache and compute new values if needed */
    if (n > max_n) {
        size_t needed = (size_t)(n + 1);
        /* Reallocate to required size */
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        cache = tmp;
        /* Compute recurrence from max_n+1 up to n */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            cache[i] =
                (seq_t)7 * cache[i - 1]
              - (seq_t)5 * cache[i - 2]
              +          cache[i - 3];
        }
        max_n = n;
    }

    return cache[n];
}
