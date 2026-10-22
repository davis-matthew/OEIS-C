/*
 * A002509  Expansion of a modular function for Gamma_0(14).
 * https://oeis.org/A002509
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = [q^(n-4)] Product_{k>=1} (1-q^k)^(-c(k)), c = period 14
 *            [-1,4,-1,4,-1,4,6,4,-1,4,-1,4,-1,0] (Euler transform;
 *            equivalently eta(z)*eta(14z)^11/(eta(2z)^5*eta(7z)^7))
 * Range:     i8 n <= 12, i16 n <= 27, i32 n <= 77, i64 n <= 245,
 *            i128 n <= 849, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *a002509_cache = NULL;
static idx_t a002509_len = 0;

static void a002509_build(idx_t N) {
    idx_t L = N + 1, k, i;
    int t, r;
    seq_t *f = (seq_t *)calloc((size_t)L, sizeof(seq_t));
    for (i = 0; i < L; i++) f[i] = 0;
    f[0] = 1;
    for (k = 1; k <= N; k++) {
        r = (int)(k % 14);
        if (r == 0) continue;
        if (r & 1) {
            if (r == 7) {
                for (t = 0; t < 6; t++)
                    for (i = k; i < L; i++) f[i] += f[i - k];
            } else {
                for (i = N; i >= k; i--) f[i] -= f[i - k];
            }
        } else {
            for (t = 0; t < 4; t++)
                for (i = k; i < L; i++) f[i] += f[i - k];
        }
    }
    if (a002509_cache) free(a002509_cache);
    a002509_cache = f;
    a002509_len = L;
}

seq_t a(idx_t n) {
    idx_t idx;
    if (n < 4) return 0;
    idx = n - 4;
    if (idx >= a002509_len) {
        idx_t N = idx + 1;
        if (N < 2 * a002509_len) N = 2 * a002509_len;
        if (N < 64) N = 64;
        a002509_build(N);
    }
    return a002509_cache[idx];
}
