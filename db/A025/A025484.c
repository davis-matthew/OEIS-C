/*
 * A025484  a(0) = 0; a(n) = a(n/5)/5 if n = a(n/5) = 0 (mod 5); a(n) =
 *          a(n-1)+1 otherwise.
 * https://oeis.org/A025484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n/5)/5 if n≡0 (mod 5) and a(n/5)≡0 (mod 5); otherwise
 *            a(n) = a(n-1)+1. a(0) = 0
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ensure_cache.h"

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* allocated capacity (in elements) */
    static idx_t computed = 0;      /* largest index whose value is known */

    /* initialise cache on first call */
    if (cache == NULL) {
        ensure_cache(0, &cache, &cap);
        cache[0] = 0;
        computed = 0;
    }

    /* extend cache to include n, computing values on the fly */
    if (n > computed) {
        ensure_cache(n, &cache, &cap);
        for (idx_t i = computed + 1; i <= n; ++i) {
            if ((i % 5) == 0) {
                idx_t k = i / 5;
                if ((cache[k] % 5) == 0)
                    cache[i] = cache[k] / 5;
                else
                    cache[i] = cache[i - 1] + 1;
            } else {
                cache[i] = cache[i - 1] + 1;
            }
        }
        computed = n;
    }

    return cache[n];
}
