/*
 * A004583  Expansion of sqrt(8) in base 8.
 * https://oeis.org/A004583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons, easy
 * Formula:   a(n) = floor(sqrt(8)*8^(n-1)) mod 8 = floor(sqrt(2)*2^(3n-2))
 *            mod 8; computed via the bits of sqrt(2) using an exact binary
 *            digit-by-digit square root
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint32_t *SR = 0;      /* r = floor(sqrt(2) * 2^k) */
static uint32_t *SM = 0;      /* rem = 2*4^k - r^2 */
static size_t SW = 0;         /* words allocated for SR and SM */
static unsigned char *SB = 0; /* SB[i] = bit i of sqrt(2) (SB[0] = integer part) */
static size_t SNB = 0;        /* number of bits computed */
static size_t SBCAP = 0;

static void sq_ensure_words(size_t w) {
    if (w <= SW) return;
    size_t nw = SW ? SW : 16;
    while (nw < w) nw *= 2;
    SR = (uint32_t *)realloc(SR, nw * sizeof(uint32_t));
    SM = (uint32_t *)realloc(SM, nw * sizeof(uint32_t));
    memset(SR + SW, 0, (nw - SW) * sizeof(uint32_t));
    memset(SM + SW, 0, (nw - SW) * sizeof(uint32_t));
    SW = nw;
}

/* word i of T = 4*r + 1 */
static uint32_t sq_tword(size_t i) {
    uint32_t w = SR[i] << 2;
    if (i > 0) w |= SR[i - 1] >> 30;
    if (i == 0) w |= 1u;
    return w;
}

static void sq_extend(size_t need) {
    if (SNB == 0) {
        sq_ensure_words(16);
        SR[0] = 1;
        SM[0] = 1;
        SBCAP = 1024;
        SB = (unsigned char *)malloc(SBCAP);
        SB[0] = 1;
        SNB = 1;
    }
    if (need > SBCAP) {
        size_t nc = SBCAP;
        while (nc < need) nc *= 2;
        SB = (unsigned char *)realloc(SB, nc);
        SBCAP = nc;
    }
    while (SNB < need) {
        size_t w = (SNB + 8) / 32 + 2;
        sq_ensure_words(w + 1);
        size_t i;
        /* rem <<= 2 */
        uint32_t carry = 0;
        for (i = 0; i < w; i++) {
            uint32_t v = SM[i];
            SM[i] = (v << 2) | carry;
            carry = v >> 30;
        }
        /* compare rem with T = 4r+1 */
        int ge = 1;
        for (i = w; i-- > 0;) {
            uint32_t t = sq_tword(i);
            if (SM[i] != t) { ge = SM[i] > t; break; }
        }
        unsigned bit = 0;
        if (ge) {
            uint64_t borrow = 0;
            for (i = 0; i < w; i++) {
                uint64_t t = (uint64_t)sq_tword(i) + borrow;
                uint64_t m = SM[i];
                if (m >= t) { SM[i] = (uint32_t)(m - t); borrow = 0; }
                else { SM[i] = (uint32_t)(m + 0x100000000ULL - t); borrow = 1; }
            }
            bit = 1;
        }
        /* r = 2r + bit */
        carry = bit;
        for (i = 0; i < w; i++) {
            uint32_t v = SR[i];
            SR[i] = (v << 1) | carry;
            carry = v >> 31;
        }
        SB[SNB++] = (unsigned char)bit;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    size_t top = (size_t)(3 * n - 2); /* index of least significant bit */
    sq_extend(top + 1);
    unsigned v = 0;
    int j;
    for (j = 2; j >= 0; j--) {
        v <<= 1;
        if (top >= (size_t)j) v |= SB[top - (size_t)j];
    }
    return (seq_t)v;
}
