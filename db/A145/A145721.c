/*
 * A145721  Numbers n such that there exists x in N with (x+1)^3-x^3=127*n^2.
 * https://oeis.org/A145721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n+2) = 2030*a(n+1) - a(n); a(1) = 1; a(2) = 2029
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: cache[i] holds a(i+1) so a(1) is cache[0] */
    static seq_t *cache   = NULL;
    static idx_t cache_len = 0;      /* number of terms currently stored */
    static size_t cache_cap = 0;     /* allocated capacity in seq_t elements */

    if (n < 1) {                      /* undefined for offset 0 */
        return (seq_t)0;
    }

    /* ensure cache large enough to hold a(n) */
    if (n > cache_len) {
        size_t needed = (size_t)n;
        if (!cache) {
            cache_cap = needed > 256 ? needed : 256;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            if (!cache) { /* allocation failure; should not happen in tests */
                return (seq_t)0;
            }
            cache_len = 0;
        } else if (needed > cache_cap) {
            cache_cap = needed;
            seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (!tmp) { /* realloc failure */
                return (seq_t)0;
            }
            cache = tmp;
        }

        /* compute missing terms up to n */
        for (idx_t i = cache_len; i < n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else if (i == 1) {
                cache[1] = (seq_t)2029;
            } else {
                cache[i] = (seq_t)2030 * cache[i-1] - cache[i-2];
            }
        }
        cache_len = n;
    }

    return cache[n - 1];
}
