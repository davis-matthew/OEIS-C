/*
 * A000951  Number of forests with n nodes and height at most 4.
 * https://oeis.org/A000951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   E.g.f.: exp(x*exp(x*exp(x*exp(x*exp(x))))). Let F_0 = exp(x),
 *            F_j = exp(x*F_{j-1}), and f_j(m) = m! [x^m] F_j. Then f_0(m) =
 *            1, f_j(0) = 1, and f_j(m) = Sum_{k=1..m} C(m-1,k-1) * k *
 *            f_{j-1}(k-1) * f_j(m-k); a(n) = f_4(n)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   grow_arr_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/grow_arr_local.h"

static seq_t *F[5];
static seq_t *row;      /* row[j] = C(done-1, j) */
static idx_t done = 0;  /* f_j(m) computed for m <= done */
static idx_t cap = 0;

seq_t a(idx_t n) {
    idx_t m, k, j;
    int lev;
    if (n + 1 > cap) {
        idx_t nc = cap ? cap : 16;
        while (nc < n + 1) nc *= 2;
        for (lev = 0; lev < 5; lev++) F[lev] = grow_arr_local(F[lev], cap, nc);
        row = grow_arr_local(row, cap, nc);
        cap = nc;
    }
    if (done == 0) {
        for (lev = 0; lev < 5; lev++) F[lev][0] = 1;
    }
    for (m = done + 1; m <= n; m++) {
        /* update row from C(m-2, .) to C(m-1, .) */
        if (m == 1) {
            row[0] = 1;
        } else {
            row[m - 1] = 0;
            for (j = m - 1; j >= 1; j--) row[j] += row[j - 1];
        }
        F[0][m] = 1;
        for (lev = 1; lev < 5; lev++) {
            seq_t s = 0;
            for (k = 1; k <= m; k++) {
                s += row[k - 1] * ((seq_t)k * F[lev - 1][k - 1]) * F[lev][m - k];
            }
            F[lev][m] = s;
        }
        done = m;
    }
    return F[4][n];
}
