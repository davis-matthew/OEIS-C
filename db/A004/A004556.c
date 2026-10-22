/*
 * A004556  Expansion of sqrt(5) in base 3.
 * https://oeis.org/A004556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   a(n) = floor(sqrt(5)*3^(n-1)) mod 3, computed digit by digit
 *            with the base-3 square-root recurrence r(1) = 2, m(1) = 1
 *            (remainder 5 - r^2); r(k+1) = 3*r(k) + d, m(k+1) = 9*m(k) -
 *            d*(6*r(k)+d), where d is the largest of {0,1,2} that keeps
 *            m(k+1) >= 0; a(k+1) = d
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bn_mul_add
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/bn_mul_add.h"

static uint32_t *R_, *M_, *T_, *S_;
static int lr_, lm_, cap_;
static unsigned char *dig_;
static idx_t nd_, dcap_;

static int bn_cmp(const uint32_t *a, int la, const uint32_t *b, int lb) {
    int i;
    if (la != lb) return la < lb ? -1 : 1;
    for (i = la - 1; i >= 0; i--) {
        if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
    }
    return 0;
}

static int bn_sub(uint32_t *a, int la, const uint32_t *b, int lb) {
    int64_t br = 0;
    int i;
    for (i = 0; i < la; i++) {
        int64_t v = (int64_t)a[i] - br - (i < lb ? (int64_t)b[i] : 0);
        if (v < 0) { v += ((int64_t)1 << 32); br = 1; } else br = 0;
        a[i] = (uint32_t)v;
    }
    while (la > 0 && a[la - 1] == 0) la--;
    return la;
}

static void ensure_cap(int need) {
    if (need <= cap_) return;
    {
        int nc = cap_ ? cap_ : 16;
        while (nc < need) nc *= 2;
        R_ = (uint32_t *)realloc(R_, sizeof(uint32_t) * (size_t)nc);
        M_ = (uint32_t *)realloc(M_, sizeof(uint32_t) * (size_t)nc);
        T_ = (uint32_t *)realloc(T_, sizeof(uint32_t) * (size_t)nc);
        S_ = (uint32_t *)realloc(S_, sizeof(uint32_t) * (size_t)nc);
        cap_ = nc;
    }
}

static void step(void) {
    int lt, ls = 0;
    uint32_t d;
    uint32_t *tmp;
    ensure_cap((lr_ > lm_ ? lr_ : lm_) + 4);
    lt = bn_mul_add(T_, M_, lm_, 9, 0);
    for (d = 2; d >= 1; d--) {
        ls = bn_mul_add(S_, R_, lr_, 6 * d, d * d);
        if (bn_cmp(S_, ls, T_, lt) <= 0) break;
    }
    if (d == 0) ls = 0;
    if (ls) lt = bn_sub(T_, lt, S_, ls);
    tmp = M_; M_ = T_; T_ = tmp;
    lm_ = lt;
    lr_ = bn_mul_add(R_, R_, lr_, 3, d);
    if (nd_ >= dcap_) {
        dcap_ = dcap_ ? dcap_ * 2 : 64;
        dig_ = (unsigned char *)realloc(dig_, (size_t)dcap_);
    }
    dig_[nd_++] = (unsigned char)d;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    if (nd_ == 0) {
        ensure_cap(16);
        R_[0] = 2; lr_ = 1;
        M_[0] = 1; lm_ = 1;
        dcap_ = 64;
        dig_ = (unsigned char *)malloc((size_t)dcap_);
        dig_[0] = 2;
        nd_ = 1;
    }
    while (nd_ < n) step();
    return (seq_t)dig_[n - 1];
}
