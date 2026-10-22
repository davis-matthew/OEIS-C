/*
 * A007549  Number of increasing rooted connected graphs where every block is
 *          a complete graph.
 * https://oeis.org/A007549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, eigen, nice
 * Formula:   E.g.f. A(x) = Sum a(n) x^n/n! satisfies A'(x) = exp(exp(A(x)) -
 *            1). With P = exp(A), Q = A' = exp(P - 1): p(0) = 1, q(0) = 1,
 *            p(m+1) = Sum_{k=0..m} binomial(m,k) * q(k) * p(m-k), q(m+1) =
 *            Sum_{k=0..m} binomial(m,k) * p(k+1) * q(m-k), a(n) = q(n-1)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/grow_zero.h"

static seq_t *P_ = 0;   /* p(0..cnt-1) */
static seq_t *Q_ = 0;   /* q(0..cnt-1) */
static seq_t *C_ = 0;   /* binomial row cnt-1 */
static idx_t cap_ = 0;
static idx_t cnt_ = 0;

static void ensure_cap(idx_t need) {
    if (cap_ < need) {
        idx_t nc = cap_ * 2;
        if (nc < need) nc = need;
        if (nc < 64) nc = 64;
        P_ = grow_zero(P_, cap_, nc);
        Q_ = grow_zero(Q_, cap_, nc);
        C_ = grow_zero(C_, cap_, nc);
        cap_ = nc;
    }
}

seq_t a(idx_t n) {
    idx_t k, m;
    if (n < 1) return (seq_t)0;
    ensure_cap(n + 2);
    if (cnt_ == 0) {
        P_[0] = (seq_t)1;
        Q_[0] = (seq_t)1;
        C_[0] = (seq_t)1;
        cnt_ = 1;
    }
    while (cnt_ < n) {
        seq_t sp = (seq_t)0, sq = (seq_t)0;
        m = cnt_ - 1;              /* C_ holds binomial row m */
        for (k = 0; k <= m; k++)
            sp += C_[k] * Q_[k] * P_[m - k];
        P_[m + 1] = sp;
        for (k = 0; k <= m; k++)
            sq += C_[k] * P_[k + 1] * Q_[m - k];
        Q_[m + 1] = sq;
        /* advance binomial row to m+1 */
        C_[m + 1] = (seq_t)1;
        for (k = m; k >= 1; k--)
            C_[k] += C_[k - 1];
        cnt_++;
    }
    return Q_[n - 1];
}
