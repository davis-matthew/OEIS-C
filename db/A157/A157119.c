/*
 * A157119  Nonnegative values x of solutions (x, y) to the Diophantine
 *          equation x^2+(x+103)^2 = y^2.
 * https://oeis.org/A157119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-3) - a(n-6) + 206 for n > 6; initial values:
 *            a(1)=0, a(2)=84, a(3)=105, a(4)=309, a(5)=765, a(6)=884
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_len = 0;          /* number of allocated slots (index max+1) */

    if (cache == NULL) {                  /* first call: initialise */
        cache_len = 7;                    /* indices 0..6 */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;             /* allocation failure */

        cache[0] = (seq_t)0;             /* unused */
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)84;
        cache[3] = (seq_t)105;
        cache[4] = (seq_t)309;
        cache[5] = (seq_t)765;
        cache[6] = (seq_t)884;
    }

    if (n < 1) return 0;                  /* undefined for n<1, return 0 safely */

    if ((size_t)n >= cache_len) {         /* need to grow and compute new terms */
        size_t new_len = (size_t)n + 1;   /* allocate up to index n */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;              /* allocation failure */
        cache = tmp;

        for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
            if (i > 6) {                  /* recurrence applies only for i>6 */
                seq_t v1 = cache[(size_t)i - 3];
                seq_t v2 = cache[(size_t)i - 6];
                cache[(size_t)i] = (seq_t)6 * v1 - v2 + (seq_t)206;
            }
            /* indices 1..6 are already initialised; nothing to do */
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
