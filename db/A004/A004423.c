/*
 * A004423  Expansion of (Sum_{n=-inf..inf} x^(n^2))^(-22).
 * https://oeis.org/A004423
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = [x^n] theta(x)^(-22), theta(x) = Sum_{k=-inf..inf}
 *            x^(k^2) = 1 + 2*Sum_{k>=1} x^(k^2); computed by dividing 1 by
 *            theta 22 times: h(n) = h_prev(n) - 2*Sum_{k>=1, k^2<=n} h(n-k^2)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 8, i64 n <= 22, i128 n <= 67,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *cached = NULL;
static idx_t cached_len = 0;

seq_t a(idx_t n) {
    idx_t need = n + 1;
    if (cached == NULL || cached_len < need) {
        idx_t len = need;
        if (len < 2 * cached_len) len = 2 * cached_len;
        if (len < 64) len = 64;
        seq_t *h = alloc_zero(len);
        idx_t i, k;
        int p;
        h[0] = 1;
        for (p = 0; p < 22; p++) {
            for (i = 1; i < len; i++) {
                seq_t s = 0;
                for (k = 1; k * k <= i; k++) s += h[i - k * k];
                h[i] -= s * 2;
            }
        }
        if (cached != NULL) release(cached, cached_len);
        cached = h;
        cached_len = len;
    }
    return cached[n];
}
