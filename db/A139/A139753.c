/*
 * A139753  a(2n) = next cube after a(2n-1), a(2n+1) = next square after
 *          a(2n).
 * https://oeis.org/A139753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for n≥2 a(2k) = next cube after a(2k‑1) a(2k+1) = next
 *            square after a(2k)
 * Range:     u8 n <= 6, u16 n <= 46, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_cube_root, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/floor_cube_root.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined, but safe return */

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;    /* number of terms stored */
    static size_t cache_cap = 0;    /* capacity of cache array */

    size_t needed = (size_t)n;

    /* grow cache if necessary */
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? (cache_cap << 1) : 64;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return 0;   /* allocation failure */
        if (cache) {
            memcpy(new_cache, cache, cache_len * sizeof(seq_t));
            free(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* initialise first term if cache empty */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;   /* a(1) = 1 */
        cache_len = 1;
    }

    /* compute missing terms up to 'n' */
    if (needed > cache_len) {
        for (size_t i = cache_len + 1; i <= needed; ++i) {
            seq_t prev = cache[i - 2];    /* previous term a(i-1) */
            if ((i & 1) == 0) {          /* even index: next cube */
                seq_t k = floor_cube_root(prev) + 1;
                cache[i - 1] = (k * k) * k;
            } else {                     /* odd index: next square */
                seq_t k = floor_sqrt(prev) + 1;
                cache[i - 1] = k * k;
            }
        }
        cache_len = needed;
    }

    return cache[n - 1];
}
