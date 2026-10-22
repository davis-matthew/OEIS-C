/*
 * A005741  Number of fanout-free Boolean functions of n variables using And,
 *          Or, Xor, Not, and Majority gates.
 * https://oeis.org/A005741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)*A005617(k), where A005617(0) =
 *            2 and A005617(n) = 2*n!*[x^n] T(x) for n >= 1. T counts
 *            read-once functions up to complement: T = x + A + X + M, A =
 *            exp(2T-A) - 1 - (2T-A), X = exp(T-X) - 1 - (T-X), M = (2/3)*T^3.
 *            The series are worked out term by term with labele...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/grow_zero.h"

static seq_t *T_ = 0, *U_ = 0, *E_ = 0, *V_ = 0, *F_ = 0, *S_ = 0, *R_ = 0;
static idx_t len_ = 0, cap_ = 0;

static void ensure(idx_t N)
{
    idx_t n, k;
    if (N < len_) return;
    if (N + 2 > cap_) {
        idx_t nc = cap_ * 2;
        if (nc < N + 2) nc = N + 2;
        if (nc < 16) nc = 16;
        T_ = grow_zero(T_, cap_, nc);
        U_ = grow_zero(U_, cap_, nc);
        E_ = grow_zero(E_, cap_, nc);
        V_ = grow_zero(V_, cap_, nc);
        F_ = grow_zero(F_, cap_, nc);
        S_ = grow_zero(S_, cap_, nc);
        R_ = grow_zero(R_, cap_, nc);
        cap_ = nc;
    }
    if (len_ == 0) {
        T_[0] = 0; U_[0] = 0; E_[0] = 1; V_[0] = 0; F_[0] = 1; S_[0] = 0;
        R_[0] = 1; /* row 0 of Pascal's triangle */
        len_ = 1;
    }
    for (n = len_; n <= N; n++) {
        /* R_ holds row n-1 */
        seq_t A = 0, X = 0, M = 0, t, s;
        for (k = 1; k < n; k++) {
            seq_t c = R_[k - 1];
            A += c * U_[k] * E_[n - k];
            X += c * V_[k] * F_[n - k];
            M += c * T_[k] * S_[n - k];
        }
        M *= 2;
        if (n == 1) t = 1;
        else t = A + X + M;
        T_[n] = t;
        U_[n] = t + t - A;
        E_[n] = A + U_[n];
        V_[n] = t - X;
        F_[n] = X + V_[n];
        /* advance R_ to row n */
        R_[n] = 0;
        for (k = n; k >= 1; k--) R_[k] += R_[k - 1];
        s = 0;
        for (k = 1; k < n; k++) s += R_[k] * T_[k] * T_[n - k];
        S_[n] = s;
    }
    len_ = N + 1;
}

seq_t a(idx_t n)
{
    seq_t sum = 2, c = 1;
    idx_t k;
    ensure(n);
    for (k = 1; k <= n; k++) {
        c = c * (seq_t)(n - k + 1) / (seq_t)k;
        sum += c * 2 * T_[k];
    }
    return sum;
}
