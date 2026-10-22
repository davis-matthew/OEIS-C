/*
 * A001760  Number of permutations of [n] with n-4 sequences.
 * https://oeis.org/A001760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = R(m) with m = n+4, where E(k) = A000111(k) (Euler zigzag
 *            numbers, from the e.g.f. sec(t)+tan(t)) and a(n) = (E(m+3) -
 *            (3m-16)*E(m+1))/3 - 2*E(m+2) + (2m-6)*E(m). This comes from the
 *            e.g.f. (1/4)u^4 - (1+t/2)u^3 + (3+t)u^2 - (7+t/2)u with u =
 *            sec(t)+tan(t), using u' = (1+u^2)/2, so u^2 = 2u...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   a1760_fixed, a1760_mul, alloc_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/a1760_fixed.h"
#include "../utils/a1760_mul.h"

static seq_t *a1760_E = 0;
static seq_t *a1760_rowA = 0;
static seq_t *a1760_rowB = 0;
static idx_t a1760_cnt = 0;
static idx_t a1760_cap = 0;

static void a1760_ensure(idx_t M)
{
    idx_t k, j;
    if (M + 2 > a1760_cap) {
        idx_t nc = a1760_cap * 2;
        idx_t i;
        seq_t *E2, *A2, *B2;
        if (nc < M + 2)
            nc = M + 2;
        if (nc < 16)
            nc = 16;
        E2 = alloc_zero(nc);
        A2 = alloc_zero(nc);
        B2 = alloc_zero(nc);
        for (i = 0; i < a1760_cnt; i++) {
            E2[i] = a1760_E[i];
            A2[i] = a1760_rowA[i];
        }
        a1760_E = E2;
        a1760_rowA = A2;
        a1760_rowB = B2;
        a1760_cap = nc;
    }
    if (a1760_cnt == 0) {
        a1760_rowA[0] = 1;
        a1760_E[0] = 1;
        a1760_cnt = 1;
    }
    for (k = a1760_cnt; k <= M; k++) {
        seq_t *cur = a1760_rowB;
        seq_t *prev = a1760_rowA;
        cur[0] = 0;
        for (j = 1; j <= k; j++)
            cur[j] = cur[j - 1] + prev[k - j];
        a1760_E[k] = cur[k];
        a1760_rowA = cur;
        a1760_rowB = prev;
        a1760_cnt = k + 1;
    }
}

seq_t a(idx_t n)
{
    idx_t m = n + 4;
    seq_t X, q, r;
    a1760_ensure(m + 3);
    X = a1760_E[m + 3] + a1760_mul((seq_t)16, a1760_E[m + 1]);
    X -= a1760_mul((seq_t)(3 * m), a1760_E[m + 1]);
    if (a1760_fixed()) {
        seq_t inv = 3;
        int i;
        for (i = 0; i < 8; i++) {
            seq_t t = (seq_t)((seq_t)2 - a1760_mul((seq_t)3, inv));
            inv = a1760_mul(inv, t);
        }
        q = a1760_mul(X, inv);
    } else {
        q = X / 3;
    }
    r = q + a1760_mul((seq_t)(2 * m - 6), a1760_E[m]);
    r -= a1760_mul((seq_t)2, a1760_E[m + 2]);
    return r;
}
