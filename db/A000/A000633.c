/*
 * A000633  Number of acyclic quaternary ammonium ions with n carbon atoms.
 * https://oeis.org/A000633
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. A(x) = Z(S4, B(x)-1), where B(x) = 1 + x*Z(S3, B(x)) is the
 *            g.f. of A000598 (ternary trees, b(0)=1); computed by a
 *            division-free multiset DP: T[j][w] = number of multisets of j
 *            nonempty ternary trees with w carbons in total, b(k) =
 *            T[0][k-1]+T[1][k-1]+T[2][k-1]+T[3][k-1], a(n) = T[4][n]
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 28, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Validated: partial, pass, 31 terms through n = 34 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

/* multichoose(b, t) = C(b+t-1, t) for t <= 4, removing the denominator's 2s and 3s before multiplying */
static seq_t mchoose(seq_t b, int t) {
    seq_t f[4];
    int i, need2, need3;
    seq_t r = 1;
    for (i = 0; i < t; i++) f[i] = b + i;
    need2 = (t >= 4) ? 3 : (t >= 2 ? 1 : 0);
    need3 = (t >= 3) ? 1 : 0;
    for (i = 0; i < t; i++)
        while (need2 > 0 && f[i] != 0 && f[i] % 2 == 0) { f[i] /= 2; need2--; }
    for (i = 0; i < t; i++)
        while (need3 > 0 && f[i] != 0 && f[i] % 3 == 0) { f[i] /= 3; need3--; }
    for (i = 0; i < t; i++) r *= f[i];
    return r;
}

static seq_t *T[5];
static seq_t *B;
static idx_t M = 0;

static void build(idx_t N) {
    idx_t k, w;
    int j, t;
    for (j = 0; j < 5; j++) T[j] = alloc_zero(N + 1);
    B = alloc_zero(N + 1);
    T[0][0] = 1;
    B[0] = 1;
    for (k = 1; k <= N; k++) {
        seq_t m[5];
        B[k] = T[0][k - 1] + T[1][k - 1] + T[2][k - 1] + T[3][k - 1];
        m[0] = 1;
        for (t = 1; t <= 4; t++) m[t] = mchoose(B[k], t);
        for (w = N; w >= k; w--) {
            for (j = 4; j >= 1; j--) {
                seq_t s = 0;
                for (t = 1; t <= j && (idx_t)t * k <= w; t++)
                    s += m[t] * T[j - t][w - (idx_t)t * k];
                T[j][w] += s;
            }
        }
    }
    M = N;
}

seq_t a(idx_t n) {
    if (n < 4) return 0;
    if (n > M) {
        idx_t N = 2 * M;
        if (N < 64) N = 64;
        if (N < n) N = n;
        build(N);
    }
    return T[4][n];
}
