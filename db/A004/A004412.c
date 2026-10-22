/*
 * A004412  Expansion of ( Sum_{n = -infinity..infinity} x^(n^2) )^(-11).
 * https://oeis.org/A004412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   G.f. 1/theta_3(x)^11. Divide by theta_3 eleven times: if g =
 *            f/theta_3, then g(n) = f(n) - 2*Sum_{j>=1, j^2<=n} g(n-j^2),
 *            starting from f = 1
 * Range:     i8 n <= 1, i16 n <= 4, i32 n <= 11, i64 n <= 35, i128 n <= 111,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *cached = NULL;
static idx_t cached_len = 0;

static void compute(idx_t len) {
    seq_t *prev = alloc_zero(len);
    prev[0] = 1;
    for (int k = 1; k <= 11; k++) {
        seq_t *cur = alloc_zero(len);
        for (idx_t m = 0; m < len; m++) {
            seq_t s = 0;
            for (idx_t j = 1; j * j <= m; j++) {
                s += cur[m - j * j];
            }
            cur[m] = prev[m] - s - s;
        }
        release(prev, len);
        prev = cur;
    }
    if (cached != NULL) {
        release(cached, cached_len);
    }
    cached = prev;
    cached_len = len;
}

seq_t a(idx_t n) {
    if (n >= cached_len) {
        idx_t len = cached_len * 2;
        if (len < n + 1) len = n + 1;
        if (len < 64) len = 64;
        compute(len);
    }
    return cached[n];
}
