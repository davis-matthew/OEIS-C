/*
 * A005647  Salié numbers.
 * https://oeis.org/A005647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   G.f. A(x) = 1/(1 - x/(1 - 2x/(1 - 5x/(1 - 8x/(1 - ...))))), an
 *            S-fraction with coefficients c(k) = ceiling(k^2/2) (A000982);
 *            a(n) is the weighted count of Dyck paths of length 2n where a
 *            down step from height k to k-1 has weight c(k), a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 21,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *a005647_cache = 0;
static idx_t a005647_count = 0;

static void a005647_build(idx_t N) {
    seq_t *row = alloc_zero(N + 2);
    seq_t *nxt = alloc_zero(N + 2);
    seq_t *res = alloc_zero(N + 1);
    idx_t m, h;
    row[0] = 1;
    res[0] = 1;
    for (m = 0; m < 2 * N; m++) {
        idx_t pmax = (m < 2 * N - m) ? m : 2 * N - m;
        idx_t m1 = m + 1;
        idx_t hm = (m1 < 2 * N - m1) ? m1 : 2 * N - m1;
        seq_t *tmp;
        for (h = 0; h <= hm; h++) {
            seq_t v = 0;
            if (h >= 1) v += row[h - 1];
            if (h + 1 <= pmax) {
                idx_t k = h + 1;
                idx_t c = (k * k + 1) / 2;
                v += row[h + 1] * c;
            }
            nxt[h] = v;
        }
        tmp = row; row = nxt; nxt = tmp;
        if ((m1 & 1) == 0) res[m1 / 2] = row[0];
    }
    release(row, N + 2);
    release(nxt, N + 2);
    if (a005647_cache) release(a005647_cache, a005647_count);
    a005647_cache = res;
    a005647_count = N + 1;
}

seq_t a(idx_t n) {
    if (n >= a005647_count) {
        idx_t N = n;
        if (N < 2 * a005647_count) N = 2 * a005647_count;
        if (N < 16) N = 16;
        a005647_build(N);
    }
    return a005647_cache[n];
}
