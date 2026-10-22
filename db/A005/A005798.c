/*
 * A005798  Expansion of (theta_2(q)/theta_3(q))^4/16 in powers of q.
 * https://oeis.org/A005798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 0, a(n) = B(n-1), where B(q) = (psi(q^2)/phi(q))^4 =
 *            psi(q^2)^4 / phi(q)^4. psi(q^2)^4 = Sum_{j>=0} sigma(2j+1)
 *            q^(2j). Dividing by phi(q) = 1 + 2*Sum_{k>=1} q^(k^2) four times
 *            uses the sparse recurrence x(m) = y(m) - 2*Sum_{k>=1, k^2<=m}
 *            x(m-k^2)
 * Range:     i8 n <= 3, i16 n <= 8, i32 n <= 22, i64 n <= 71, i128 n <= 245,
 *            big any n
 * Helpers:   divphi, grow_zero, sigma_odd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/grow_zero.h"
#include "../utils/divphi.h"
#include "../utils/sigma_odd.h"

static seq_t *x1_ = NULL, *x2_ = NULL, *x3_ = NULL, *x4_ = NULL;
static idx_t len_ = 0;

static void extend(idx_t need) {
    idx_t newlen, m;
    if (need <= len_) return;
    newlen = need;
    if (newlen < len_ + len_ / 2) newlen = len_ + len_ / 2;
    x1_ = grow_zero(x1_, len_, newlen);
    x2_ = grow_zero(x2_, len_, newlen);
    x3_ = grow_zero(x3_, len_, newlen);
    x4_ = grow_zero(x4_, len_, newlen);
    for (m = len_; m < newlen; m++) {
        seq_t y = 0;
        if (m % 2 == 0) y = (seq_t)sigma_odd((long long)m + 1);
        x1_[m] = divphi(x1_, y, m);
        x2_[m] = divphi(x2_, x1_[m], m);
        x3_[m] = divphi(x3_, x2_[m], m);
        x4_[m] = divphi(x4_, x3_[m], m);
    }
    len_ = newlen;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    extend(n);
    return x4_[n - 1];
}
