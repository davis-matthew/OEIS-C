/*
 * A004421  Expansion of (Sum_{n=-inf..inf} x^(n^2))^(-20).
 * https://oeis.org/A004421
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   G.f. 1/theta_3(x)^20, theta_3(x) = Sum_{k=-inf..inf} x^(k^2);
 *            computed by dividing 1 by theta_3 twenty times: h(n) = f(n) -
 *            2*Sum_{j>=1, j^2<=n} h(n-j^2)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 8, i64 n <= 24, i128 n <= 72,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *cache = 0;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (n >= cache_len) {
        idx_t len = n + 1;
        if (len < 2 * cache_len) len = 2 * cache_len;
        if (len < 32) len = 32;
        seq_t *buf = alloc_zero(len);
        buf[0] = 1;
        for (int m = 0; m < 20; m++) {
            for (idx_t i = 0; i < len; i++) {
                seq_t s = buf[i];
                for (idx_t j = 1; j * j <= i; j++) {
                    s -= buf[i - j * j];
                    s -= buf[i - j * j];
                }
                buf[i] = s;
            }
        }
        if (cache) release(cache, cache_len);
        cache = buf;
        cache_len = len;
    }
    return cache[n];
}
