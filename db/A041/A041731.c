/*
 * A041731  Denominators of continued fraction convergents to sqrt(385).
 * https://oeis.org/A041731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0) = 1, * a(1) = 1, a(2) = 2, a(3) = 3, a(4) = 5, a(5) = 8,
 *            a(6) = 29, a(7) = 37, * a(8) = 103, a(9) = 140, a(10) = 523,
 *            a(11) = 663, a(12) = 1186, a(13) = 1849, * a(14) = 3035, a(15) =
 *            4884, a(16) = 188627, ... * For n > 31: a(n) = 191662 * a(n-16)
 *            - a(n-32) (recurrence)
 * Range:     u8 none, u16 n <= 15, u32 n <= 31, u64 n <= 61, u128 n <= 118,
 *            big any n
 * Helpers:   compute_base_terms
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/compute_base_terms.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t computed = 0;  /* last index computed */

    /* Allocate initial cache */
    if (cache_size == 0) {
        cache_size = (n + 1 > 32 ? n + 1 : 32);
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure – not expected */
        compute_base_terms(cache);
        computed = 31;                 /* base terms up to index 31 are ready */
    }

    /* Grow cache if needed for the current n */
    if (n + 1 > cache_size) {
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure – not expected */
        cache_size = n + 1;
    }

    /* Compute missing terms using recurrence, if any */
    if (n > computed) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            /* recurrence: a(i) = 191662*a(i-16) - a(i-32)   for i>31 */
            cache[i] = (seq_t)191662 * cache[i - 16] - cache[i - 32];
        }
        computed = n;
    }

    return cache[n];
}
