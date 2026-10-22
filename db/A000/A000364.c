/*
 * A000364  Euler (or secant or "Zig") numbers: e.g.f. (even powers only)
 *          sec(x) = 1/cos(x).
 * https://oeis.org/A000364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, core
 * Formula:   a(n) = A000111(2n), computed with the Seidel-Entringer triangle:
 *            E(0,0) = 1, E(m,0) = 0, E(m,k) = E(m,k-1) + E(m-1,m-k),
 *            A000111(m) = E(m,m)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *a364_vals = NULL;
static idx_t a364_have = 0;

seq_t a(idx_t n) {
    if (n < a364_have)
        return a364_vals[n];

    idx_t N = n;
    if (N < 2 * a364_have) N = 2 * a364_have;
    if (N < 16) N = 16;
    idx_t M = 2 * N;

    seq_t *p = (seq_t *)calloc((size_t)(M + 2), sizeof(seq_t));
    seq_t *q = (seq_t *)calloc((size_t)(M + 2), sizeof(seq_t));
    seq_t *R = (seq_t *)calloc((size_t)(N + 1), sizeof(seq_t));

    p[0] = 1;
    R[0] = 1;
    for (idx_t m = 1; m <= M; m++) {
        q[0] = 0;
        for (idx_t k = 1; k <= m; k++)
            q[k] = q[k - 1] + p[m - k];
        seq_t *t = p; p = q; q = t;
        if ((m & 1) == 0)
            R[m / 2] = p[m];
    }

    free(p);
    free(q);
    if (a364_vals != NULL) free(a364_vals);
    a364_vals = R;
    a364_have = N + 1;
    return a364_vals[n];
}
