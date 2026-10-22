/*
 * A004410  Expansion of ( Sum_{n = -infinity..infinity} x^(n^2) )^(-9).
 * https://oeis.org/A004410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   G.f. = 1/theta_3(x)^9, theta_3(x) = 1 + 2*Sum_{k>=1} x^(k^2);
 *            computed by dividing by theta_3 nine times: b(m) = b(m) -
 *            2*Sum_{k>=1, k^2<=m} b(m-k^2), starting from b = 1
 * Range:     i8 n <= 1, i16 n <= 4, i32 n <= 13, i64 n <= 40, i128 n <= 130,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n < cache_size) {
        return cache[n];
    }

    idx_t new_size = n + 1;
    if (new_size < 2 * cache_size) new_size = 2 * cache_size;

    seq_t *g = alloc_zero(new_size);
    g[0] = 1;

    for (int p = 0; p < 9; p++) {
        for (idx_t m = 1; m < new_size; m++) {
            seq_t s = 0;
            for (idx_t k = 1; k * k <= m; k++) {
                s += g[m - k * k];
            }
            g[m] -= 2 * s;
        }
    }

    if (cache) {
        release(cache, cache_size);
    }
    cache = g;
    cache_size = new_size;

    return cache[n];
}
