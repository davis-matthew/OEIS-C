/*
 * A007303  Earliest monotonic sequence fixed (apart from signs) under
 *          reversion.
 * https://oeis.org/A007303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, eigen
 * Formula:   greedy search using the rule that B(x) = -A(-x) is the series
 *            reversion of A(x). Initial values a(1)=1, a(2)=2, a(3)=4. For
 *            each odd m >= 5, a(m-1) is the least value > a(m-2) whose forced
 *            a(m) = (m+1)*a(m-1) + (S_even - S_odd)/2 is greater than a(m-1).
 *            Here S_even (S_odd) = Sum_{k even (odd)} a(...
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 18, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   alloc_zero, release, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_fixed_width.h"

static seq_t *A_ = NULL, *P_ = NULL;
static idx_t cap_ = 0;

/* P_[k*W + j] = [x^j] A(x)^k */
static void col(idx_t j, idx_t W)
{
    idx_t k, i;
    P_[W + j] = A_[j];
    for (k = 2; k <= j; k++) {
        seq_t s = 0;
        for (i = 1; i + k <= j + 1; i++)
            s += A_[i] * P_[(k - 1) * W + (j - i)];
        P_[k * W + j] = s;
    }
}

static void build(idx_t M)
{
    idx_t W = M + 1, m, k;
    bool fixed = seq_is_fixed_width();
    A_ = alloc_zero(W);
    P_ = alloc_zero(W * W);
    cap_ = M;
    P_[0] = 1;
    A_[1] = 1; A_[2] = 2; A_[3] = 4;
    col(1, W); col(2, W); col(3, W);
    for (m = 5; m <= M; m += 2) {
        seq_t so = 0, se = 0, t, q, d;
        bool neg;
        A_[m - 1] = 0; A_[m] = 0;
        col(m - 1, W); col(m, W);
        for (k = 2; k <= m; k++) {
            if (k & 1) so += A_[k] * P_[k * W + m];
            else se += A_[k] * P_[k * W + m];
        }
        if (fixed) {
            d = se - so;
            neg = ((d >> (8 * sizeof(seq_t) - 1)) & 1) != 0;
            if (neg) d = (seq_t)0 - d;
        } else {
            neg = so > se;
            d = neg ? so - se : se - so;
        }
        t = A_[m - 2] + 1;
        if (neg) {
            q = d / ((seq_t)m * 2) + 1;
            if (q > t) t = q;
        }
        A_[m - 1] = t;
        if (neg) A_[m] = (seq_t)(m + 1) * t - d / 2;
        else A_[m] = (seq_t)(m + 1) * t + d / 2;
        col(m - 1, W); col(m, W);
    }
}

seq_t a(idx_t n)
{
    if (n > cap_) {
        idx_t M = cap_ * 2;
        if (M < n) M = n;
        if (M < 15) M = 15;
        if (!(M & 1)) M++;
        if (A_) {
            release(A_, cap_ + 1);
            release(P_, (cap_ + 1) * (cap_ + 1));
        }
        build(M);
    }
    return A_[n];
}
