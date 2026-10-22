/*
 * A004570  Expansion of sqrt(7) in base 3.
 * https://oeis.org/A004570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   a(n) = floor(sqrt(7) * 3^(n-1)) mod 3, computed with a
 *            digit-by-digit base-3 square root on native multi-limb integers:
 *            p(1) = 2, r(1) = 3; d = max{d in 0..2 : d*(6*p + d) <= 9*r}, p'
 *            = 3*p + d, r' = 9*r - d*(6*p + d); a(n) = d of step n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   muladd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/muladd.h"

static uint32_t *P_, *R_, *T_, *U_;
static size_t cap_ = 0, pl_ = 0, rl_ = 0;
static unsigned char *D_ = NULL;
static idx_t dn_ = 0, dcap_ = 0;

static void ensure_cap(size_t need) {
    if (need <= cap_) return;
    size_t nc = need * 2 + 8;
    P_ = (uint32_t *)realloc(P_, nc * sizeof(uint32_t));
    R_ = (uint32_t *)realloc(R_, nc * sizeof(uint32_t));
    T_ = (uint32_t *)realloc(T_, nc * sizeof(uint32_t));
    U_ = (uint32_t *)realloc(U_, nc * sizeof(uint32_t));
    cap_ = nc;
}

static int cmp_big(const uint32_t *a, size_t al, const uint32_t *b, size_t bl) {
    while (al > 0 && a[al - 1] == 0) al--;
    while (bl > 0 && b[bl - 1] == 0) bl--;
    if (al != bl) return al < bl ? -1 : 1;
    while (al > 0) {
        al--;
        if (a[al] != b[al]) return a[al] < b[al] ? -1 : 1;
    }
    return 0;
}

/* a -= b, requires a >= b; returns normalized length */
static size_t sub_big(uint32_t *a, size_t al, const uint32_t *b, size_t bl) {
    int64_t borrow = 0;
    size_t i;
    for (i = 0; i < al; i++) {
        int64_t v = (int64_t)a[i] - borrow - (i < bl ? (int64_t)b[i] : 0);
        if (v < 0) { v += ((int64_t)1 << 32); borrow = 1; } else borrow = 0;
        a[i] = (uint32_t)v;
    }
    while (al > 0 && a[al - 1] == 0) al--;
    return al;
}

static void step(void) {
    size_t mx = (pl_ > rl_ ? pl_ : rl_) + 3;
    ensure_cap(mx);
    size_t tl = muladd(T_, R_, rl_, 9, 0);
    unsigned d;
    size_t ul = muladd(U_, P_, pl_, 12, 4);
    if (cmp_big(U_, ul, T_, tl) <= 0) {
        d = 2;
        tl = sub_big(T_, tl, U_, ul);
    } else {
        ul = muladd(U_, P_, pl_, 6, 1);
        if (cmp_big(U_, ul, T_, tl) <= 0) {
            d = 1;
            tl = sub_big(T_, tl, U_, ul);
        } else {
            d = 0;
            while (tl > 0 && T_[tl - 1] == 0) tl--;
        }
    }
    uint32_t *tmp = R_; R_ = T_; T_ = tmp;
    rl_ = tl;
    pl_ = muladd(P_, P_, pl_, 3, d);
    if (dn_ >= dcap_) {
        dcap_ = dcap_ * 2 + 64;
        D_ = (unsigned char *)realloc(D_, (size_t)dcap_);
    }
    D_[dn_++] = (unsigned char)d;
}

seq_t a(idx_t n) {
    if (dn_ == 0) {
        ensure_cap(8);
        P_[0] = 2; pl_ = 1;
        R_[0] = 3; rl_ = 1;
        dcap_ = 64;
        D_ = (unsigned char *)malloc((size_t)dcap_);
        D_[0] = 2;
        dn_ = 1;
    }
    while (dn_ < n) step();
    return (seq_t)D_[n - 1];
}
