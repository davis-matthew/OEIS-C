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
 * Helpers:   alloc_zero, mchoose
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/mchoose.h"

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
