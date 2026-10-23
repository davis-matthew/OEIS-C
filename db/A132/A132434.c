/*
 * A132434  a(n) = A132433(n) - 33.
 * https://oeis.org/A132434
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) + 232 for n >= 4, a(3) = 104
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 3) return 0;   /* below offset – undefined */

    /* cache: cache[k-3] holds a(k), for k >= 3 */
    static seq_t *cache = NULL;
    static idx_t last_computed = 2;   /* last index known in cache (value of n) */

    if (cache == NULL) {
        /* initialise cache with a(3) and a(4) */
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (cache == NULL) return 0;   /* allocation fail – unlikely */

        cache[0] = (seq_t)104;          /* a(3) */
        cache[1] = (seq_t)1064;         /* a(4) */
        last_computed = 4;
    }

    /* extend cache if necessary */
    if (n > last_computed) {
        idx_t new_size = n - 2;  /* number of terms to store (from a(3) to a(n)) */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return 0; /* reallocation fail – unlikely */
        cache = tmp;

        /* compute new entries using a(n) = 8*a(n-1) + 232 */
        for (idx_t k = last_computed + 1; k <= n; ++k) {
            idx_t idx = k - 3;          /* index in cache for term k */
            cache[idx] = 8 * cache[idx - 1] + (seq_t)232;
        }
        last_computed = n;
    }

    return cache[n - 3];
}
