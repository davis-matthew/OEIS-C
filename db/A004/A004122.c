/*
 * A004122  Generalized weak orders on n points.
 * https://oeis.org/A004122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   E.g.f. 1/(2-exp(x)*exp(exp(x)-1)) = 1/(2 - Sum_{k>=0} Bell(k+1)
 *            x^k/k!), so a(n) = Sum_{k=1..n} binomial(n,k)*Bell(k+1)*a(n-k),
 *            a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *A_memo = 0;
static idx_t A_cap = 0;

static void ensure(idx_t n) {
    idx_t m, i, j, k, r;
    seq_t *bell, *p, *q, *row, *t, *av;
    if (A_memo && n <= A_cap) return;
    m = (A_memo && 2 * A_cap > n) ? 2 * A_cap : n;
    if (m < 8) m = 8;

    /* Bell numbers 0..m+1 via the Bell triangle */
    bell = alloc_zero(m + 2);
    p = alloc_zero(m + 3);
    q = alloc_zero(m + 3);
    p[0] = 1;
    bell[0] = 1;
    for (r = 1; r <= m + 1; r++) {
        q[0] = p[r - 1];
        for (j = 1; j <= r; j++) q[j] = q[j - 1] + p[j - 1];
        bell[r] = q[0];
        t = p; p = q; q = t;
    }

    /* a(i) via rolling Pascal row */
    av = alloc_zero(m + 1);
    row = alloc_zero(m + 1);
    av[0] = 1;
    row[0] = 1;
    for (i = 1; i <= m; i++) {
        row[i] = 1;
        for (j = i - 1; j >= 1; j--) row[j] += row[j - 1];
        seq_t s = 0;
        for (k = 1; k <= i; k++) s += row[k] * bell[k + 1] * av[i - k];
        av[i] = s;
    }

    release(bell, m + 2);
    release(p, m + 3);
    release(q, m + 3);
    release(row, m + 1);
    if (A_memo) release(A_memo, A_cap + 1);
    A_memo = av;
    A_cap = m;
}

seq_t a(idx_t n) {
    ensure(n);
    return A_memo[n];
}
