/*
 * A003818  a(1)=a(2)=1, a(n+1) = (a(n)^3 +1)/a(n-1).
 * https://oeis.org/A003818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+1) = (a(n)^3 + 1)/a(n-1), a(1) = a(2) = 1. Computed as x*q +
 *            (x*r + 1)/y, where x = a(n), y = a(n-1), q = x^2 / y and r = x^2
 *            % y, so no intermediate value exceeds the result
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   next_term
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/next_term.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (n <= 2) return 1;

    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc((cache_cap + 1) * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 1;
        cache_len = 2;
    }

    if (n > cache_len) {
        if (n > cache_cap) {
            idx_t new_cap = cache_cap;
            while (new_cap < n) new_cap *= 2;
            seq_t *new_cache = (seq_t *)malloc((new_cap + 1) * sizeof(seq_t));
            for (idx_t i = 0; i <= cache_len; i++) new_cache[i] = cache[i];
            free(cache);
            cache = new_cache;
            cache_cap = new_cap;
        }
        for (idx_t i = cache_len + 1; i <= n; i++) {
            cache[i] = next_term(cache[i - 1], cache[i - 2]);
        }
        cache_len = n;
    }

    return cache[n];
}
