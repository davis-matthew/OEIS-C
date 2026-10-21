/*
 * A217729  Trajectory of 40 under the map n-> A006369(n).
 * https://oeis.org/A217729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the (n‑1)th term in the trajectory of 40 under the map *
 *            n -> A006369(n). The function A006369 is defined by: * if n mod
 *            3 == 0: return 2*n/3 * else: return round(4*n/3), which can be
 *            computed as * 4*(n/3) + (n%3==1 ? 1 : 3)
 * Range:     u8 n <= 14, u16 n <= 86, u32 n <= 195, u64 n <= 653,
 *            u128 n <= 1515, big any n
 * Helpers:   A006369
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/A006369.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;      /* number of terms currently stored */

/* Public function required by the problem statement */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;     /* undefined offset */

    /* initialise cache with the first term a(1)=40 */
    if (cache == NULL) {
        cache_len = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) abort();         /* allocation failure */
        cache[0] = (seq_t)40;        /* a(1) */
    }

    /* extend cache up to n if necessary */
    if (n > cache_len) {
        /* double the capacity until it is big enough */
        idx_t new_cap = cache_len ? cache_len : 1;
        while (new_cap < n) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!new_cache) abort();     /* allocation failure */
        cache = new_cache;

        /* compute new terms */
        for (idx_t i = cache_len; i < n; ++i) {
            cache[i] = A006369(cache[i - 1]);
        }
        cache_len = n;
    }

    return cache[n - 1];
}
