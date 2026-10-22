/*
 * A005221  Number of Dyck paths of knight moves.
 * https://oeis.org/A005221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, walk
 * Formula:   G.f. A(x) = x^2*B(x)^2/(1-x*B(x)), where B(x) = A005220(x)
 *            counts Dyck paths of knight moves (1,2),(2,1),(1,-2),(2,-1) from
 *            (0,0) to (k,0) that never go below the x-axis. B is computed by
 *            a column DP over these paths. Then a(n) = Sum_{i=0..n-2}
 *            [x^i]B^2 * [x^(n-2-i)]C, where C = 1/(1-x*B), C_0 =...
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 27, u64 n <= 50, u128 n <= 95,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *Bs = NULL, *B2s = NULL, *Cs = NULL;
static idx_t cap = 0; /* arrays hold indices 0..cap-1 */

static void build(idx_t N) {
    idx_t sz = N + 3, x, y, k, j;
    seq_t *p2 = alloc_zero(sz), *p1 = alloc_zero(sz), *cur = alloc_zero(sz), *tmp;
    seq_t *B = alloc_zero(N + 1), *B2 = alloc_zero(N + 1), *C = alloc_zero(N + 1);

    p1[0] = 1;          /* column 0 */
    B[0] = 1;
    for (x = 1; x <= N; x++) {
        idx_t lim = 2 * x < 2 * (N - x) ? 2 * x : 2 * (N - x);
        idx_t zl = 2 * x + 4 < N + 2 ? 2 * x + 4 : N + 2;
        for (y = 0; y <= zl; y++) cur[y] = 0;
        for (y = 0; y <= lim; y++) {
            seq_t v = p1[y + 2];
            v += p2[y + 1];
            if (y >= 2) v += p1[y - 2];
            if (y >= 1) v += p2[y - 1];
            cur[y] = v;
        }
        B[x] = cur[0];
        tmp = p2; p2 = p1; p1 = cur; cur = tmp;
    }
    release(p2, sz); release(p1, sz); release(cur, sz);

    for (k = 0; k <= N; k++) {
        seq_t s = 0;
        for (j = 0; j <= k; j++) s += B[j] * B[k - j];
        B2[k] = s;
    }
    C[0] = 1;
    for (k = 1; k <= N; k++) {
        seq_t s = 0;
        for (j = 0; j < k; j++) s += B[j] * C[k - 1 - j];
        C[k] = s;
    }

    if (cap) {
        release(Bs, cap); release(B2s, cap); release(Cs, cap);
    }
    Bs = B; B2s = B2; Cs = C;
    cap = N + 1;
}

seq_t a(idx_t n) {
    idx_t i;
    seq_t s = 0;
    if (n < 2) return 0;
    if (n >= cap) {
        idx_t N = n;
        if (N < 2 * cap) N = 2 * cap;
        if (N < 64) N = 64;
        build(N);
    }
    for (i = 0; i <= n - 2; i++) s += B2s[i] * Cs[n - 2 - i];
    return s;
}
