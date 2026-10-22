/*
 * A003238  Number of rooted trees with n vertices in which vertices at the
 *          same level have the same degree.
 * https://oeis.org/A003238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, eigen
 * Formula:   a(1) = 1; a(n+1) = Sum_{k|n} a(k)
 * Range:     u8 n <= 26, u16 n <= 118, u32 n <= 902, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   A_grow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/A_grow.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t computed = 0;

    if (cache == NULL) {
        cache = A_grow(NULL, 0, 16);
        cache_len = 16;
        cache[0] = 1;
        computed = 1;
    }

    if (n > cache_len) {
        idx_t new_len = cache_len;
        while (new_len < n) new_len *= 2;
        cache = A_grow(cache, cache_len, new_len);
        cache_len = new_len;
    }

    if (n > computed) {
        for (idx_t i = computed + 1; i <= n; i++) {
            seq_t sum = 0;
            idx_t m = i - 1;
            for (idx_t k = 1; k * k <= m; k++) {
                if (m % k == 0) {
                    sum += cache[k - 1];
                    if (k != m / k) {
                        sum += cache[m / k - 1];
                    }
                }
            }
            cache[i - 1] = sum;
        }
        computed = n;
    }

    return cache[n - 1];
}
