/*
 * A004589  Expansion of sqrt(10) in base 6.
 * https://oeis.org/A004589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   a(n) = floor(sqrt(10) * 6^(n-1)) mod 6, i.e. the n-th base-6
 *            digit of sqrt(10), computed by exact digit-by-digit square root
 *            in base 6 (memoized)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint32_t *P = NULL, *R = NULL, *T = NULL;
static size_t lcap = 0, plen = 0, rlen = 0, tlen = 0;
static unsigned char *dig = NULL;
static idx_t ndig = 0, dcap = 0;

static void ensure_limbs(size_t need) {
    if (need <= lcap) return;
    size_t nc = lcap ? lcap : 16;
    while (nc < need) nc *= 2;
    P = (uint32_t *)realloc(P, nc * sizeof(uint32_t));
    R = (uint32_t *)realloc(R, nc * sizeof(uint32_t));
    T = (uint32_t *)realloc(T, nc * sizeof(uint32_t));
    lcap = nc;
}

static void mul_small(uint32_t *x, size_t *len, uint32_t m, uint32_t add) {
    uint64_t carry = add;
    size_t i;
    for (i = 0; i < *len; i++) {
        uint64_t v = (uint64_t)x[i] * m + carry;
        x[i] = (uint32_t)v;
        carry = v >> 32;
    }
    if (carry) x[(*len)++] = (uint32_t)carry;
}

static int cmp_limbs(const uint32_t *a, size_t alen, const uint32_t *b, size_t blen) {
    while (alen > 0 && a[alen - 1] == 0) alen--;
    while (blen > 0 && b[blen - 1] == 0) blen--;
    if (alen != blen) return alen < blen ? -1 : 1;
    while (alen > 0) {
        alen--;
        if (a[alen] != b[alen]) return a[alen] < b[alen] ? -1 : 1;
    }
    return 0;
}

static void sub_limbs(uint32_t *a, size_t *alen, const uint32_t *b, size_t blen) {
    uint64_t borrow = 0;
    size_t i;
    for (i = 0; i < *alen; i++) {
        uint64_t bv = (i < blen ? b[i] : 0) + borrow;
        uint64_t av = a[i];
        if (av >= bv) { a[i] = (uint32_t)(av - bv); borrow = 0; }
        else { a[i] = (uint32_t)(av + ((uint64_t)1 << 32) - bv); borrow = 1; }
    }
    while (*alen > 0 && a[*alen - 1] == 0) (*alen)--;
}

static void next_digit(void) {
    ensure_limbs((plen > rlen ? plen : rlen) + 8);
    if (ndig == 0) {
        R[0] = 10; rlen = 1; plen = 0;
    } else {
        mul_small(R, &rlen, 36, 0);
    }
    uint32_t d;
    for (d = 5; d >= 1; d--) {
        memcpy(T, P, plen * sizeof(uint32_t));
        tlen = plen;
        mul_small(T, &tlen, 12, d);
        mul_small(T, &tlen, d, 0);
        if (cmp_limbs(T, tlen, R, rlen) <= 0) break;
    }
    if (d >= 1) sub_limbs(R, &rlen, T, tlen);
    mul_small(P, &plen, 6, d);
    while (plen > 0 && P[plen - 1] == 0) plen--;
    if (ndig >= dcap) {
        idx_t nc = dcap ? dcap * 2 : 64;
        dig = (unsigned char *)realloc(dig, (size_t)nc);
        dcap = nc;
    }
    dig[ndig++] = (unsigned char)d;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    while (ndig < n) next_digit();
    return (seq_t)dig[n - 1];
}
