/*
 * A006196  Number of leftist trees with n leaves.
 * https://oeis.org/A006196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   G.f. A(x) = x + A(x*A(x)). Let f(x) = x*A(x) and y_j = f^(j)(x),
 *            the j-th iterate. Then A(x) = Sum_{j>=0} y_j and A(y_j) = S_j =
 *            Sum_{i>=j} y_i, so y_0 = x, y_{j+1} = y_j * S_j, a(n) = Sum_j
 *            [x^n] y_j, a(0) = 0, a(1) = 1. y_j has valuation 2^j, so each
 *            coefficient depends only on earlier coeffici...
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 28, u64 n <= 51, u128 n <= 95,
 *            big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

static seq_t *Y_[64];   /* Y_[j][t] = [x^t] y_j */
static seq_t *S_[64];   /* S_[j][t] = [x^t] Sum_{i>=j} y_i ; S_[0] = A */
static int LV_ = 0;     /* number of levels allocated */
static idx_t cap_ = 0;  /* length of each level array */
static idx_t done_ = 0; /* coefficients 0..done_-1 are computed */

static void reserve(idx_t n) {
    if (n < cap_) return;
    idx_t newcap = 2 * cap_;
    if (newcap < n + 1) newcap = n + 1;
    if (newcap < 64) newcap = 64;
    int newLV = 0;
    while (newLV < 63 && ((idx_t)1 << newLV) < newcap) newLV++;
    for (int j = 0; j < LV_; j++) {
        Y_[j] = grow_zero(Y_[j], cap_, newcap);
        S_[j] = grow_zero(S_[j], cap_, newcap);
    }
    for (int j = LV_; j < newLV; j++) {
        Y_[j] = alloc_zero(newcap);
        S_[j] = alloc_zero(newcap);
    }
    LV_ = newLV;
    cap_ = newcap;
}

seq_t a(idx_t n) {
    if (n >= done_) {
        reserve(n);
        for (idx_t t = done_; t <= n; t++) {
            int top = 0;
            Y_[0][t] = (t == 1) ? 1 : 0;
            for (int j = 1; j < LV_ && ((idx_t)1 << j) <= t; j++) {
                idx_t h = (idx_t)1 << (j - 1);
                seq_t s = 0;
                for (idx_t q = h; q + h <= t; q++)
                    s += Y_[j - 1][q] * S_[j - 1][t - q];
                Y_[j][t] = s;
                top = j;
            }
            seq_t acc = 0;
            for (int j = top; j >= 0; j--) {
                acc += Y_[j][t];
                S_[j][t] = acc;
            }
        }
        done_ = n + 1;
    }
    return S_[0][n];
}
