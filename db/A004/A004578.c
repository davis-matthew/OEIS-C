/*
 * A004578  Expansion of sqrt(8) in base 3.
 * https://oeis.org/A004578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   a(n) = floor(sqrt(8) * 3^(n-1)) mod 3, i.e. a(n) =
 *            isqrt(8*9^(n-1)) mod 3; the digits come from a digit-by-digit
 *            square root in base 3, using a native multi-limb integer (limbs
 *            of 3^18), and are memoized
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define LIMB_B 387420489ULL /* 3^18 */

static uint64_t *P = NULL, *R = NULL, *T = NULL;
static size_t plen = 0, rlen = 0, tlen = 0, cap = 0;
static unsigned char *dig = NULL;
static idx_t ndig = 0, dcap = 0;

static void ensure_cap(size_t need) {
    if (need <= cap) return;
    size_t nc = cap ? cap : 16;
    while (nc < need) nc *= 2;
    P = (uint64_t *)realloc(P, nc * sizeof(uint64_t));
    R = (uint64_t *)realloc(R, nc * sizeof(uint64_t));
    T = (uint64_t *)realloc(T, nc * sizeof(uint64_t));
    memset(P + cap, 0, (nc - cap) * sizeof(uint64_t));
    memset(R + cap, 0, (nc - cap) * sizeof(uint64_t));
    memset(T + cap, 0, (nc - cap) * sizeof(uint64_t));
    cap = nc;
}

static void mul_small_add(uint64_t *x, size_t *len, uint64_t m, uint64_t add) {
    uint64_t carry = add;
    size_t i;
    for (i = 0; i < *len; i++) {
        uint64_t v = x[i] * m + carry;
        x[i] = v % LIMB_B;
        carry = v / LIMB_B;
    }
    while (carry) {
        x[(*len)++] = carry % LIMB_B;
        carry /= LIMB_B;
    }
    while (*len > 0 && x[*len - 1] == 0) (*len)--;
}

static int cmp_big(const uint64_t *a, size_t alen, const uint64_t *b, size_t blen) {
    if (alen != blen) return alen < blen ? -1 : 1;
    size_t i = alen;
    while (i > 0) {
        i--;
        if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
    }
    return 0;
}

static void sub_big(uint64_t *a, size_t *alen, const uint64_t *b, size_t blen) {
    uint64_t borrow = 0;
    size_t i;
    for (i = 0; i < *alen; i++) {
        uint64_t s = (i < blen ? b[i] : 0) + borrow;
        if (a[i] >= s) { a[i] -= s; borrow = 0; }
        else { a[i] = a[i] + LIMB_B - s; borrow = 1; }
    }
    while (*alen > 0 && a[*alen - 1] == 0) (*alen)--;
}

static void extend_to(idx_t n) {
    if (n > dcap) {
        idx_t nc = dcap ? dcap : 64;
        while (nc < n) nc *= 2;
        dig = (unsigned char *)realloc(dig, (size_t)nc);
        dcap = nc;
    }
    while (ndig < n) {
        size_t mx = plen > rlen ? plen : rlen;
        ensure_cap(mx + 4);
        /* bring down next pair of base-3 digits: "22" (=8) first, then "00" */
        mul_small_add(R, &rlen, 9, ndig == 0 ? 8 : 0);
        int d;
        for (d = 2; d >= 1; d--) {
            memcpy(T, P, plen * sizeof(uint64_t));
            tlen = plen;
            mul_small_add(T, &tlen, 6, (uint64_t)d);
            mul_small_add(T, &tlen, (uint64_t)d, 0);
            if (cmp_big(T, tlen, R, rlen) <= 0) break;
        }
        if (d > 0) sub_big(R, &rlen, T, tlen);
        else d = 0;
        mul_small_add(P, &plen, 3, (uint64_t)d);
        dig[ndig++] = (unsigned char)d;
    }
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    extend_to(n);
    return (seq_t)dig[n - 1];
}
