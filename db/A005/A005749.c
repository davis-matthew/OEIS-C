/*
 * A005749  Cascade-realizable Boolean functions of n variables.
 * https://oeis.org/A005749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * A005613(k), where A005613(0)
 *            = A005613(1) = 2 and, for k >= 2, A005613(k) = 2*U(k) + X(k)
 *            with U(k) = 2^k + Sum_{m=2..k-1} binomial(k,m) * 2^(k-m) * (U(m)
 *            + X(m)) X(k) = 2 * (1 + Sum_{m=2..k-1} binomial(k,m) * U(m)) (U
 *            = cascades whose top gate is AND (= those...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/grow_zero.h"
#include <stdlib.h>

static seq_t *U_ = NULL, *X_ = NULL, *c_ = NULL, *row = NULL, *p2 = NULL, *res = NULL;
static idx_t done = 0;   /* indices 0..done-1 computed */
static idx_t cap = 0;

seq_t a(idx_t n) {
    if (n >= done) {
        if (n + 1 > cap) {
            idx_t nc = n + 1;
            U_ = grow_zero(U_, cap, nc);
            X_ = grow_zero(X_, cap, nc);
            c_ = grow_zero(c_, cap, nc);
            row = grow_zero(row, cap, nc);
            p2 = grow_zero(p2, cap, nc);
            res = grow_zero(res, cap, nc);
            cap = nc;
        }
        for (idx_t i = done; i <= n; i++) {
            /* Pascal row i */
            if (i == 0) {
                row[0] = 1;
            } else {
                for (idx_t k = i; k >= 1; k--) row[k] += row[k - 1];
            }
            if (i == 0) p2[0] = 1;
            else p2[i] = p2[i - 1] * 2;

            if (i <= 1) {
                U_[i] = 0;
                X_[i] = 0;
                c_[i] = 2;
            } else {
                seq_t u = p2[i];
                seq_t x = 1;
                for (idx_t m = 2; m < i; m++) {
                    u += row[m] * p2[i - m] * (U_[m] + X_[m]);
                    x += row[m] * U_[m];
                }
                x *= 2;
                U_[i] = u;
                X_[i] = x;
                c_[i] = u * 2 + x;
            }
            seq_t s = 0;
            for (idx_t k = 0; k <= i; k++) s += row[k] * c_[k];
            res[i] = s;
        }
        done = n + 1;
    }
    return res[n];
}
