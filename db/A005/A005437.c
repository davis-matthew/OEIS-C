/*
 * A005437  Column of Kempner tableau.
 * https://oeis.org/A005437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = E(n, ceil(n/2)), where E are the Entringer numbers
 *            (A008281): E(0,0) = 1, E(m,0) = 0 for m >= 1, E(m,k) = E(m,k-1)
 *            + E(m-1,m-k) for 1 <= k <= m
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 15, u64 n <= 24, u128 n <= 38,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/grow_zero.h"

static seq_t *P = NULL;   /* last computed Entringer row */
static seq_t *Q = NULL;   /* scratch row */
static seq_t *R = NULL;   /* memoized results a(0..rows-1) */
static idx_t cap = 0;
static idx_t rows = 0;

seq_t a(idx_t n) {
    if (n < rows) return R[n];

    if (n + 1 > cap) {
        idx_t newcap = 2 * cap;
        if (newcap < n + 1) newcap = n + 1;
        P = grow_zero(P, cap, newcap);
        Q = grow_zero(Q, cap, newcap);
        R = grow_zero(R, cap, newcap);
        cap = newcap;
    }

    if (rows == 0) {
        P[0] = 1;
        R[0] = 1;
        rows = 1;
    }

    while (rows <= n) {
        idx_t m = rows;
        seq_t *t;
        idx_t k;
        Q[0] = 0;
        for (k = 1; k <= m; k++)
            Q[k] = Q[k - 1] + P[m - k];
        R[m] = Q[(m + 1) / 2];
        t = P; P = Q; Q = t;
        rows++;
    }

    return R[n];
}
