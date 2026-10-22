/*
 * A006679  Number of planted binary phylogenetic trees with n labels.
 * https://oeis.org/A006679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   E.g.f. A(x) satisfies A = (e^x - 1) + e^x * A^2/2, so with c(m)
 *            = Sum_{k=1..m-1} C(m-1,k-1)*a(k)*a(m-k) (the e.g.f. coefficients
 *            of A^2/2), a(n) = 1 + Sum_{k=0..n} C(n,k)*c(k) for n >= 1, a(0)
 *            = 0
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/grow_zero.h"

static seq_t *A_ = NULL;   /* a(k) */
static seq_t *C_ = NULL;   /* c(k) */
static seq_t *R_ = NULL;   /* current Pascal row: C(done-1, k) */
static idx_t cap_ = 0;
static idx_t done_ = 0;    /* a(0..done_-1) are known */

static void ensure_cap(idx_t need) {
    idx_t nc;
    if (need <= cap_) return;
    nc = cap_ ? cap_ : 16;
    while (nc < need) nc *= 2;
    A_ = grow_zero(A_, cap_, nc);
    C_ = grow_zero(C_, cap_, nc);
    R_ = grow_zero(R_, cap_, nc);
    cap_ = nc;
}

seq_t a(idx_t n) {
    idx_t m, k;
    ensure_cap(n + 2);
    if (done_ == 0) {
        A_[0] = 0;
        C_[0] = 0;
        R_[0] = 1;
        done_ = 1;
    }
    for (m = done_; m <= n; m++) {
        seq_t c = 0, d = 0;
        /* R_ holds row m-1 */
        for (k = 1; k < m; k++)
            c += R_[k - 1] * A_[k] * A_[m - k];
        C_[m] = c;
        /* advance R_ to row m */
        R_[m] = 0;
        for (k = m; k >= 1; k--)
            R_[k] += R_[k - 1];
        for (k = 0; k <= m; k++)
            d += R_[k] * C_[k];
        A_[m] = d + 1;
        done_ = m + 1;
    }
    return A_[n];
}
