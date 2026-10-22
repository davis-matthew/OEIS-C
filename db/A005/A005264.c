/*
 * A005264  Number of labeled rooted Greg trees with n nodes.
 * https://oeis.org/A005264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy, changed
 * Formula:   E.g.f. A(x) satisfies A' = exp(A)/(1-2*A). With B = A', E =
 *            exp(A): B = E + 2*A*B and E' = B*E. In e.g.f. coefficients (b(m)
 *            = a(m+1), a(0) = 0, e(0) = 1): a(m+1) = e(m) + 2*Sum_{k=1..m}
 *            C(m,k)*a(k)*a(m-k+1), e(m+1) = Sum_{k=0..m} C(m,k)*a(k+1)*e(m-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   grow_vals
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/grow_vals.h"

static seq_t *av = NULL;   /* av[k] = a(k) */
static seq_t *ev = NULL;   /* ev[k] = e(k) */
static seq_t *row = NULL;  /* row[k] = C(m,k) for current m */
static idx_t cap = 0;
static idx_t next_m = 0;   /* next m to process */

static void ensure_cap(idx_t need) {
    idx_t nc;
    if (need <= cap) return;
    nc = cap * 2;
    if (nc < need) nc = need;
    if (nc < 64) nc = 64;
    av = grow_vals(av, cap, nc);
    ev = grow_vals(ev, cap, nc);
    row = grow_vals(row, cap, nc);
    cap = nc;
}

seq_t a(idx_t n) {
    idx_t m, k;
    if (n < 1) return 0;
    ensure_cap(n + 2);
    if (next_m == 0) {
        av[0] = 0;
        ev[0] = 1;
    }
    while (next_m + 1 <= n) {
        seq_t s, t;
        m = next_m;
        /* update row to C(m, .) */
        if (m == 0) {
            row[0] = 1;
        } else {
            row[m] = 0;
            for (k = m; k >= 1; k--) row[k] += row[k - 1];
        }
        s = 0;
        for (k = 1; k <= m; k++) s += row[k] * av[k] * av[m - k + 1];
        av[m + 1] = ev[m] + 2 * s;
        t = 0;
        for (k = 0; k <= m; k++) t += row[k] * av[k + 1] * ev[m - k];
        ev[m + 1] = t;
        next_m = m + 1;
    }
    return av[n];
}
