/*
 * A094256  Expansion of x / ( (x-1)*(x^3 - 9*x^2 + 6*x - 1) ).
 * https://oeis.org/A094256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 15*a(n-2) + 10*a(n-3) - a(n-4) (n ≥ 5) Initial
 *            values: a(1)=1, a(2)=7, a(3)=34, a(4)=143
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return A094256(n).  n is 1‑based; values for n < 1 are undefined. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n < 1 */

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n > cache_size) {
        /* enlarge cache to hold terms up to n */
        idx_t new_size = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – terminate with an error code */
            exit(1);
        }
        cache = tmp;

        /* ensure that the first four base values are present */
        for (idx_t i = 0; i < 4 && i < new_size; ++i) {
            switch (i) {
                case 0: cache[0] = 1;   break;
                case 1: cache[1] = 7;   break;
                case 2: cache[2] = 34;  break;
                case 3: cache[3] = 143; break;
            }
        }

        /* compute further terms using the recurrence */
        for (idx_t i = cache_size; i < new_size; ++i) {
            if (i < 4) continue;            /* base terms already set */
            cache[i] = 7   * cache[i-1]
                     - 15  * cache[i-2]
                     + 10  * cache[i-3]
                     -     cache[i-4];
        }

        cache_size = new_size;
    }

    /* n is 1‑based, cache is 0‑based */
    return cache[n - 1];
}
