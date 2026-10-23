/*
 * A118674  Nonnegative values x of solutions (x, y) to the Diophantine
 *          equation x^2 + (x + 31)^2 = y^2.
 * https://oeis.org/A118674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-3) - a(n-6) + 62 for n > 6 initial values: a(1)=0,
 *            a(2)=9, a(3)=60, a(4)=93, a(5)=140, a(6)=429 (computes solutions
 *            to x^2 + (x+31)^2 = y^2, n≥1)
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 33, u64 n <= 71, u128 n <= 146,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache      = NULL;
    static size_t last_n     = 0;   /* maximum n already computed */
    static size_t cache_size = 0;   /* current allocated size */

    if (n == 0) return (seq_t)0;   /* undefined, but harmless */

    /* allocate or grow cache if needed */
    if ((size_t)n > cache_size) {
        size_t new_size = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);   /* allocation failed */
        cache = tmp;
        cache_size = new_size;
    }

    /* compute missing terms up to n */
    for (size_t i = last_n + 1; i <= (size_t)n; ++i) {
        switch (i) {
            case 1: cache[i-1] = 0;          break;
            case 2: cache[i-1] = 9;          break;
            case 3: cache[i-1] = 60;         break;
            case 4: cache[i-1] = 93;         break;
            case 5: cache[i-1] = 140;        break;
            case 6: cache[i-1] = 429;        break;
            default:
                /* recurrence: a(i) = 6*a(i-3) - a(i-6) + 62 */
                cache[i-1] = (seq_t)(6 * cache[i-4] - cache[i-7] + 62);
                break;
        }
    }
    last_n = (size_t)n;

    return cache[n-1];
}
