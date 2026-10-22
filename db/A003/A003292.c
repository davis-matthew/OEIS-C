/*
 * A003292  Number of 4-line partitions of n decreasing across rows.
 * https://oeis.org/A003292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f. Product_{k>=1} (1 - x^k)^-c(k), c = 1,1,2,2,2,... ;
 *            equivalently G.f. = (1-x)(1-x^2) * P(x)^2, P(x) = Product
 *            1/(1-x^k). With q = A000712 (coefficients of P(x)^2): a(n) =
 *            q(n) - q(n-1) - q(n-2) + q(n-3), where p(m) = Sum_{k>=1}
 *            (-1)^(k+1) [p(m-k(3k-1)/2) + p(m-k(3k+1)/2)], p(0) = 1, and
 *            q(m)...
 * Range:     u8 n <= 12, u16 n <= 30, u32 n <= 82, u64 n <= 250,
 *            u128 n <= 827, big any n
 * Helpers:   A_grow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/A_grow.h"

static seq_t *P_ = NULL;
static seq_t *Q_ = NULL;
static idx_t len_ = 0;

static void extend(idx_t need) {
    if (need <= len_) return;
    idx_t nl = len_ * 2;
    if (nl < need) nl = need;
    P_ = A_grow(P_, len_, nl);
    Q_ = A_grow(Q_, len_, nl);
    for (idx_t m = len_; m < nl; m++) {
        if (m == 0) {
            P_[0] = 1;
            Q_[0] = 1;
            continue;
        }
        seq_t pp = 0, pn = 0, qp = 0, qn = 0;
        for (idx_t k = 1;; k++) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > m) break;
            idx_t g2 = g1 + k;
            if (k & 1) {
                pp += P_[m - g1];
                qp += Q_[m - g1];
                if (g2 <= m) {
                    pp += P_[m - g2];
                    qp += Q_[m - g2];
                }
            } else {
                pn += P_[m - g1];
                qn += Q_[m - g1];
                if (g2 <= m) {
                    pn += P_[m - g2];
                    qn += Q_[m - g2];
                }
            }
        }
        P_[m] = pp - pn;
        seq_t t = P_[m] + qp;
        Q_[m] = t - qn;
    }
    len_ = nl;
}

seq_t a(idx_t n) {
    extend(n + 1);
    seq_t s = Q_[n];
    if (n >= 3) s += Q_[n - 3];
    seq_t t = Q_[n - 1];
    if (n >= 2) t += Q_[n - 2];
    return s - t;
}
