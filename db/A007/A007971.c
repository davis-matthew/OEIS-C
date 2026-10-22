/*
 * A007971  INVERTi transform of central trinomial coefficients (A002426).
 * https://oeis.org/A007971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 1, a(2) = 2, and for n >= 3 a(n) =
 *            (1/2)*Sum_{i=1..n-1} a(i)*a(n-i), evaluated by symmetry as
 *            Sum_{i=1..floor((n-1)/2)} a(i)*a(n-i) + [n
 *            even]*(a(n/2)/2)*a(n/2) so that no intermediate exceeds a(n)
 *            (fixed-width builds); for arbitrary precision the equivalent
 *            O(n) recurrence from...
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 25, u64 n <= 46, u128 n <= 87,
 *            big any n
 * Helpers:   seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t max_computed = 0;

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 2;

    if (cache == NULL) {
        cache_len = 128;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 2;
        max_computed = 2;
    }

    if (n >= cache_len) {
        idx_t new_len = cache_len;
        while (n >= new_len) new_len *= 2;
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        for (idx_t i = 0; i <= max_computed; i++) new_cache[i] = cache[i];
        free(cache);
        cache = new_cache;
        cache_len = new_len;
    }

    bool fixed = seq_is_fixed_width();
    for (idx_t j = max_computed + 1; j <= n; j++) {
        if (fixed) {
            seq_t sum = 0;
            for (idx_t i = 1; 2 * i < j; i++) {
                sum += cache[i] * cache[j - i];
            }
            if (j % 2 == 0) {
                seq_t h = cache[j / 2];
                sum += (h / 2) * h; /* a(k) even for k >= 2 */
            }
            cache[j] = sum;
        } else {
            seq_t t = (seq_t)(2 * j - 3) * cache[j - 1]
                    + (seq_t)(3 * j - 9) * cache[j - 2];
            cache[j] = t / (seq_t)j;
        }
    }

    if (n > max_computed) max_computed = n;

    return cache[n];
}
