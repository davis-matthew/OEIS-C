/*
 * A000432  Series-parallel numbers.
 * https://oeis.org/A000432
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. 4(2+S)(1+S)/(1-S)^5 = 4(6P^5 - 5P^4 + P^3), where P =
 *            1/(1-S) and S = g.f. of A000084, with a(3) = [x^0]. A000084(1) =
 *            1 and A000084(n) = 2*s(n) for n >= 2, where Prod_{k>=1}
 *            (1-x^k)^(-s(k)) = 1 - x + 2*Sum_{n>=1} s(n) x^n, s(1) = 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 40,
 *            big any n
 * Helpers:   grow_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/grow_arr.h"

static seq_t *s_ = 0, *S_ = 0, *c_ = 0, *p_ = 0, *q2_ = 0, *q3_ = 0, *q4_ = 0;
static idx_t L_ = 0, cap_ = 0;

static void extend(idx_t N) {
    idx_t i, k, d;
    if (N < L_) return;
    if (N >= cap_) {
        idx_t nc = cap_ ? cap_ : 64;
        while (nc <= N) nc *= 2;
        s_ = grow_arr(s_, L_, nc);
        S_ = grow_arr(S_, L_, nc);
        c_ = grow_arr(c_, L_, nc);
        p_ = grow_arr(p_, L_, nc);
        q2_ = grow_arr(q2_, L_, nc);
        q3_ = grow_arr(q3_, L_, nc);
        q4_ = grow_arr(q4_, L_, nc);
        cap_ = nc;
    }
    for (i = L_; i <= N; i++) {
        if (i == 0) {
            s_[0] = 0; S_[0] = 0; c_[0] = 0;
            p_[0] = 1; q2_[0] = 1; q3_[0] = 1; q4_[0] = 1;
            continue;
        }
        seq_t D = 0;
        for (d = 1; d < i; d++)
            if (i % d == 0) D += (seq_t)d * s_[d];
        if (i == 1) {
            s_[1] = 1;
        } else {
            seq_t T = D;
            for (k = 1; k < i; k++) T += c_[k] * S_[i - k];
            s_[i] = T / (seq_t)i;
        }
        c_[i] = (seq_t)i * s_[i] + D;
        S_[i] = (i == 1) ? (seq_t)1 : (seq_t)2 * s_[i];

        seq_t t = 0;
        for (k = 1; k <= i; k++) t += S_[k] * p_[i - k];
        p_[i] = t;
        t = 0;
        for (k = 0; k <= i; k++) t += p_[k] * p_[i - k];
        q2_[i] = t;
        t = 0;
        for (k = 0; k <= i; k++) t += q2_[k] * p_[i - k];
        q3_[i] = t;
        t = 0;
        for (k = 0; k <= i; k++) t += q3_[k] * p_[i - k];
        q4_[i] = t;
    }
    L_ = N + 1;
}

seq_t a(idx_t n) {
    idx_t N, k;
    if (n < 3) return 0;
    N = n - 3;
    extend(N);
    seq_t q5 = 0;
    for (k = 0; k <= N; k++) q5 += q4_[k] * p_[N - k];
    seq_t r = (seq_t)24 * q5 + (seq_t)4 * q3_[N];
    r -= (seq_t)20 * q4_[N];
    return r;
}
