/*
 * A041633  Denominators of continued fraction convergents to sqrt(335).
 * https://oeis.org/A041633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 1208*a(n-4) - a(n-8) for n>7, a(0) = 1, a(1) = 3, a(2) =
 *            10, a(3) = 33, a(4) = 1198, a(5) = 3627, a(6) = 12079, a(7) =
 *            39864
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static idx_t computed = 0;          /* highest index already computed */

    /* Ensure the cache is large enough for index n */
    if (cache_size < (size_t)(n + 1)) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();              /* allocation failed */
        cache = tmp;
        cache_size = new_size;
    }

    /* Base values for indices 0..7 */
    static const seq_t base[8] = {1, 3, 10, 33, 1198, 3627, 12079, 39864};

    if (n < 8) {                         /* direct lookup for first 8 terms */
        return base[n];
    }

    /* Make sure base values are stored in the cache for recurrence use */
    if (computed < 7) {
        for (idx_t i = 0; i <= 7; ++i) {
            cache[i] = base[i];
        }
        computed = 7;
    }

    /* Compute missing terms up to n using the recurrence */
    for (idx_t k = computed + 1; k <= n; ++k) {
        cache[k] = (seq_t)1208 * cache[k - 4] - cache[k - 8];
    }
    computed = n;

    return cache[n];
}
