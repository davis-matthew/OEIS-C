/*
 * A007258  McKay-Thompson series of class 6E for Monster (and, apart from
 *          signs, of class 12B).
 * https://oeis.org/A007258
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = [q^(n+1)] of prod_{k>=1} (1-q^k)^e(k), plus 5 when n = 0,
 *            where e(k) = 5,4,6,4,5,0 for k == 1,2,3,4,5,0 (mod 6); i.e. 5 +
 *            eta(q)^5*eta(q^3)/(eta(q^2)*eta(q^6)^5) shifted by q^-1. With
 *            c(0) = 1: m*c(m) = -Sum_{j=1..m} s(j)*c(m-j), s(j) = Sum_{d|j}
 *            d*e(d)
 * Range:     i8 n <= 12, i16 n <= 45, i32 n <= 153, i64 n <= 546,
 *            i128 n <= 1000, big any n
 * Helpers:   alloc_zero, e_of, release, seq_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_big.h"
#include "../utils/e_of.h"

static seq_t *C = NULL;
static idx_t L = 0;

static void build(idx_t N) {
    idx_t M, i, j, k;
    seq_t *nc;
    if (N <= L) return;
    M = (2 * L > N) ? 2 * L : N;
    if (M < 64) M = 64;
    nc = alloc_zero(M);
    if (seq_is_big()) {
        long long *sig = (long long *)calloc((size_t)M, sizeof(long long));
        for (k = 1; k < M; k++) {
            long long w = (long long)k * e_of(k);
            if (w == 0) continue;
            for (j = k; j < M; j += k) sig[j] += w;
        }
        for (i = 0; i < L; i++) nc[i] = C[i];
        if (L == 0) nc[0] = 1;
        for (i = (L == 0 ? 1 : L); i < M; i++) {
            seq_t acc = 0;
            for (j = 1; j <= i; j++) {
                if (sig[j] != 0) acc += (seq_t)sig[j] * nc[i - j];
            }
            nc[i] = -acc / (seq_t)i;
        }
        free(sig);
    } else {
        nc[0] = 1;
        for (k = 1; k < M; k++) {
            int e = e_of(k), t;
            for (t = 0; t < e; t++) {
                for (i = M - 1; i >= k; i--) nc[i] -= nc[i - k];
            }
        }
    }
    if (C != NULL) release(C, L);
    C = nc;
    L = M;
}

seq_t a(idx_t n) {
    seq_t v;
    if (n < -1) return 0;
    build(n + 2);
    v = C[n + 1];
    if (n == 0) v += 5;
    return v;
}
