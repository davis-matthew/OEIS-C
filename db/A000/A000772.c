/*
 * A000772  E.g.f. exp(tan(x) + sec(x) - 1).
 * https://oeis.org/A000772
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+1) = Sum_{k=0..n} binomial(n,k) * E(k+1) * a(n-k), a(0) = 1,
 *            where E(m) = A000111(m) are the Euler zigzag numbers (e.g.f.
 *            sec(x)+tan(x)), since A' = (sec(x)^2 + sec(x)*tan(x)) * A for A
 *            = exp(tan(x)+sec(x)-1). E(m) is computed with the
 *            Seidel-Entringer triangle: T(0,0) = 1, T(m,0) = 0, T(m,k)...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 36,
 *            big any n
 * Helpers:   a306_alloc0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a306_alloc0.h"

static seq_t *memo_a = 0;
static idx_t memo_top = 0; /* memo_a[0..memo_top] valid when memo_a != 0 */

static void build(idx_t M) {
    seq_t *av = a306_alloc0(M + 1);
    seq_t *E = a306_alloc0(M + 2);
    seq_t *r0 = a306_alloc0(M + 2);
    seq_t *r1 = a306_alloc0(M + 2);
    seq_t *C = a306_alloc0(M + 2);
    seq_t *prev = r0, *cur = r1, *tmp;
    idx_t m, k;

    prev[0] = 1;
    E[0] = 1;
    for (m = 1; m <= M + 1; m++) {
        cur[0] = 0;
        for (k = 1; k <= m; k++)
            cur[k] = cur[k - 1] + prev[m - k];
        E[m] = cur[m];
        tmp = prev; prev = cur; cur = tmp;
    }

    av[0] = 1;
    C[0] = 1; /* C holds row m of Pascal's triangle */
    for (m = 0; m < M; m++) {
        seq_t s = 0;
        for (k = 0; k <= m; k++)
            s += C[k] * E[k + 1] * av[m - k];
        av[m + 1] = s;
        for (k = m + 1; k >= 1; k--)
            C[k] = C[k] + C[k - 1];
    }

    memo_a = av;
    memo_top = M;
}

seq_t a(idx_t n) {
    if (memo_a == 0 || n > memo_top) {
        idx_t M = 32;
        if (memo_a != 0 && 2 * memo_top > M) M = 2 * memo_top;
        if (n > M) M = n;
        build(M);
    }
    return memo_a[n];
}
