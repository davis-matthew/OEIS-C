/*
 * A004408  Expansion of ( Sum_{n = -infinity..infinity} x^(n^2) )^(-7).
 * https://oeis.org/A004408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   G.f. = 1/theta_3(x)^7, theta_3(x) = 1 + 2*Sum_{k>=1} x^(k^2);
 *            computed by dividing 1 by theta_3 seven times: g(m) = f(m) -
 *            2*Sum_{k>=1} g(m-k^2)
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 15, i64 n <= 48, i128 n <= 160,
 *            big any n
 * Helpers:   release, zalloc_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/zalloc_arr.h"
#include "../utils/release.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;

static void build(idx_t len) {
    seq_t *arr = zalloc_arr(len);
    idx_t m, k, rep;
    arr[0] = 1;
    for (rep = 0; rep < 7; rep++) {
        for (m = 1; m < len; m++) {
            idx_t kmax = 0;
            seq_t s = 0;
            while ((kmax + 1) * (kmax + 1) <= m) kmax++;
            /* sum from largest k down to keep partial sums small */
            for (k = kmax; k >= 1; k--) {
                s += arr[m - k * k];
            }
            arr[m] -= 2 * s;
        }
    }
    if (cache) release(cache, cache_len);
    cache = arr;
    cache_len = len;
}

seq_t a(idx_t n) {
    if (n >= cache_len) {
        idx_t len = n + 1;
        if (len < 2 * cache_len) len = 2 * cache_len;
        if (len < 64) len = 64;
        build(len);
    }
    return cache[n];
}
