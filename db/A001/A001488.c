/*
 * A001488  Expansion of (Product_{j>=1} (1-(-x)^j) - 1)^10 in powers of x.
 * https://oeis.org/A001488
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = [x^(n-10)] g(x)^10, where Product_{j>=1} (1-(-x)^j) - 1 =
 *            x*g(x), and by Euler's pentagonal theorem Product_{j>=1}
 *            (1-(-x)^j) = Sum_{k} (-1)^(k+k(3k-1)/2) x^(k(3k-1)/2)
 * Range:     i8 n <= 13, i16 n <= 32, i32 n <= 9384, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *h_cache = 0;
static idx_t h_len = 0;

static void build(idx_t len) {
    idx_t M = len - 1;
    idx_t cap = 0, cnt = 0, k;
    idx_t *ex;
    int *sg;
    seq_t *h;
    int pw;

    for (k = 1; k * (3 * k - 1) / 2 - 1 <= M; k++) cap += 2;
    ex = (idx_t *)malloc((cap + 1) * sizeof(idx_t));
    sg = (int *)malloc((cap + 1) * sizeof(int));
    for (k = 1; k * (3 * k - 1) / 2 - 1 <= M; k++) {
        idx_t e1 = k * (3 * k - 1) / 2;
        idx_t e2 = k * (3 * k + 1) / 2;
        ex[cnt] = e1 - 1;
        sg[cnt] = ((k + e1) & 1) ? -1 : 1;
        cnt++;
        if (e2 - 1 <= M) {
            ex[cnt] = e2 - 1;
            sg[cnt] = ((k + e2) & 1) ? -1 : 1;
            cnt++;
        }
    }

    h = alloc_zero(len);
    h[0] = 1;
    for (pw = 0; pw < 10; pw++) {
        idx_t m = M + 1;
        while (m-- > 0) {
            seq_t s = 0;
            idx_t t;
            for (t = 0; t < cnt && ex[t] <= m; t++) {
                if (sg[t] > 0) s += h[m - ex[t]];
                else s -= h[m - ex[t]];
            }
            h[m] = s;
        }
    }
    free(ex);
    free(sg);

    if (h_cache) release(h_cache, h_len);
    h_cache = h;
    h_len = len;
}

seq_t a(idx_t n) {
    idx_t m;
    if (n < 10) return 0;
    m = n - 10;
    if (m >= h_len) {
        idx_t len = m + 1;
        if (len < 2 * h_len) len = 2 * h_len;
        if (len < 64) len = 64;
        build(len);
    }
    return h_cache[m];
}
