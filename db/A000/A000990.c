/*
 * A000990  Number of plane partitions of n with at most two rows.
 * https://oeis.org/A000990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f. = (1-x) / Product_{m>=1} (1-x^m)^2, so a(n) = r(n) -
 *            r(n-1), a(0) = 1, where r = A000712 = 1/E(x)^2 and E(x) =
 *            Product_{m>=1} (1-x^m) = Sum_k (-1)^k x^(k(3k-1)/2). Euler's
 *            pentagonal recurrence is used twice: q(n) = Sum_{k>=1}
 *            (-1)^(k+1) [q(n-k(3k-1)/2) + q(n-k(3k+1)/2)], q(0) = 1 (q =
 *            1/E)...
 * Range:     u8 n <= 10, u16 n <= 27, u32 n <= 75, u64 n <= 235,
 *            u128 n <= 792, big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/grow_zero.h"

static seq_t *Q = NULL;
static seq_t *R = NULL;
static idx_t cnt = 0;
static idx_t cap = 0;

static void ensure(idx_t n) {
    idx_t m;
    if (n < cnt) return;
    if (n + 1 > cap) {
        idx_t nc = cap ? cap * 2 : 64;
        while (nc < n + 1) nc *= 2;
        Q = grow_zero(Q, cap, nc);
        R = grow_zero(R, cap, nc);
        cap = nc;
    }
    for (m = cnt; m <= n; m++) {
        seq_t pq = 0, nq = 0, pr = 0, nr = 0;
        idx_t k;
        if (m == 0) {
            Q[0] = 1;
            R[0] = 1;
            continue;
        }
        for (k = 1;; k++) {
            idx_t g1 = k * (3 * k - 1) / 2;
            idx_t g2 = k * (3 * k + 1) / 2;
            if (g1 > m) break;
            if (k & 1) {
                pq += Q[m - g1];
                pr += R[m - g1];
                if (g2 <= m) {
                    pq += Q[m - g2];
                    pr += R[m - g2];
                }
            } else {
                nq += Q[m - g1];
                nr += R[m - g1];
                if (g2 <= m) {
                    nq += Q[m - g2];
                    nr += R[m - g2];
                }
            }
        }
        Q[m] = pq - nq;
        pr += Q[m];
        R[m] = pr - nr;
    }
    cnt = n + 1;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    ensure(n);
    return R[n] - R[n - 1];
}
