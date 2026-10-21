/*
 * A095992  a(1) = 30; for n > 1, a(n+1) = a(n) + {product of nonzero digits
 *          of a(n)}.
 * https://oeis.org/A095992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A095992:
   a(1) = 30;
   a(n+1) = a(n) + Π_{d∈digits(a(n)), d≠0} d
*/

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    /* static cache: cache[i] holds a(i), 1-based */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (cache_size < n) {
        /* need to enlarge cache to size n+1 (index 0 unused) */
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_cache) {
            /* out of memory – abort */
            abort();
        }

        if (cache == NULL) {
            /* first allocation */
            new_cache[0] = 0;        /* unused */
            new_cache[1] = (seq_t)30;
            cache_size = 1;
        } else {
            /* copy existing values */
            memcpy(new_cache, cache, (cache_size + 1) * sizeof(seq_t));
            free(cache);
        }

        /* compute missing terms up to n */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            seq_t prod = (seq_t)1;
            seq_t temp = new_cache[i - 1];
            while (temp != 0) {
                seq_t d = temp % (seq_t)10;
                if (d != 0)
                    prod *= d;
                temp /= (seq_t)10;
            }
            new_cache[i] = new_cache[i - 1] + prod;
        }

        cache_size = n;
        cache = new_cache;
    }

    return cache[n];
}
