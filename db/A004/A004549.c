/*
 * A004549  Expansion of sqrt(3) in base 4.
 * https://oeis.org/A004549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons, changed
 * Formula:   a(n) = floor(sqrt(3) * 4^(n-1)) mod 4 = isqrt(3 * 16^(n-1)) mod
 *            4; the base-2 digits of sqrt(3) are produced by the
 *            digit-by-digit square-root method on native multiword integers,
 *            and pairs of bits form the base-4 digits
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint32_t *R_ = 0, *Y_ = 0, *T_ = 0;
static size_t cap_ = 0;
static unsigned char *bits_ = 0;
static size_t nbits_ = 0, bitcap_ = 0;

static void ensure_cap(size_t L) {
    if (L <= cap_) return;
    size_t nc = cap_ ? cap_ : 16;
    while (nc < L) nc *= 2;
    R_ = (uint32_t *)realloc(R_, nc * sizeof(uint32_t));
    Y_ = (uint32_t *)realloc(Y_, nc * sizeof(uint32_t));
    T_ = (uint32_t *)realloc(T_, nc * sizeof(uint32_t));
    memset(R_ + cap_, 0, (nc - cap_) * sizeof(uint32_t));
    memset(Y_ + cap_, 0, (nc - cap_) * sizeof(uint32_t));
    memset(T_ + cap_, 0, (nc - cap_) * sizeof(uint32_t));
    cap_ = nc;
}

static void step(void) {
    uint32_t c = (nbits_ == 0) ? 3u : 0u;
    size_t L = (nbits_ + 4) / 32 + 2;
    size_t i;
    ensure_cap(L);
    /* R = 4R + c */
    uint32_t carry = c;
    for (i = 0; i < L; i++) {
        uint32_t w = R_[i];
        R_[i] = (w << 2) | carry;
        carry = w >> 30;
    }
    /* T = 4Y + 1 */
    carry = 1;
    for (i = 0; i < L; i++) {
        uint32_t w = Y_[i];
        T_[i] = (w << 2) | carry;
        carry = w >> 30;
    }
    /* compare R and T */
    int ge = 1;
    for (i = L; i-- > 0;) {
        if (R_[i] != T_[i]) { ge = R_[i] > T_[i]; break; }
    }
    uint32_t b = 0;
    if (ge) {
        uint64_t borrow = 0;
        for (i = 0; i < L; i++) {
            uint64_t d = (uint64_t)R_[i] - T_[i] - borrow;
            R_[i] = (uint32_t)d;
            borrow = (d >> 63) & 1;
        }
        b = 1;
    }
    /* Y = 2Y + b */
    carry = b;
    for (i = 0; i < L; i++) {
        uint32_t w = Y_[i];
        Y_[i] = (w << 1) | carry;
        carry = w >> 31;
    }
    if (nbits_ >= bitcap_) {
        size_t nc = bitcap_ ? bitcap_ * 2 : 256;
        bits_ = (unsigned char *)realloc(bits_, nc);
        bitcap_ = nc;
    }
    bits_[nbits_++] = (unsigned char)b;
}

seq_t a(idx_t n) {
    size_t need = (n <= 1) ? 1 : (size_t)(2 * n - 1);
    while (nbits_ < need) step();
    if (n <= 1) return (seq_t)bits_[0];
    unsigned d = 2u * bits_[2 * n - 3] + bits_[2 * n - 2];
    return (seq_t)d;
}
