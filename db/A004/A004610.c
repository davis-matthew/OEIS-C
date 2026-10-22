/*
 * A004610  Expansion of sqrt(6) in base 3.
 * https://oeis.org/A004610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   a(n) = floor(sqrt(6) * 3^(n-1)) mod 3, i.e. a(n) =
 *            floor(sqrt(6*9^(n-1))) mod 3, computed exactly by digit-by-digit
 *            base-3 integer square root
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint32_t *P = 0, *R = 0, *T1 = 0, *T2 = 0;
static size_t lp = 0, lr = 0, l1 = 0, l2 = 0, cap = 0;
static unsigned char *dig = 0;
static idx_t nd = 0, dcap = 0;

static void ensure_cap(size_t need) {
    if (need <= cap) return;
    size_t nc = need * 2 + 8;
    P = (uint32_t *)realloc(P, nc * sizeof(uint32_t));
    R = (uint32_t *)realloc(R, nc * sizeof(uint32_t));
    T1 = (uint32_t *)realloc(T1, nc * sizeof(uint32_t));
    T2 = (uint32_t *)realloc(T2, nc * sizeof(uint32_t));
    cap = nc;
}

static void mul_add_small(uint32_t *x, size_t *len, uint32_t m, uint32_t add) {
    uint64_t carry = add;
    size_t i;
    for (i = 0; i < *len; i++) {
        uint64_t v = (uint64_t)x[i] * m + carry;
        x[i] = (uint32_t)v;
        carry = v >> 32;
    }
    if (carry) x[(*len)++] = (uint32_t)carry;
}

static int cmp_big(const uint32_t *a, size_t la, const uint32_t *b, size_t lb) {
    size_t i;
    if (la != lb) return la < lb ? -1 : 1;
    for (i = la; i-- > 0;) {
        if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
    }
    return 0;
}

static void sub_big(uint32_t *a, size_t *la, const uint32_t *b, size_t lb) {
    int64_t borrow = 0;
    size_t i;
    for (i = 0; i < *la; i++) {
        int64_t v = (int64_t)a[i] - borrow - (i < lb ? (int64_t)b[i] : 0);
        if (v < 0) { v += ((int64_t)1 << 32); borrow = 1; } else borrow = 0;
        a[i] = (uint32_t)v;
    }
    while (*la > 0 && a[*la - 1] == 0) (*la)--;
}

static void step(void) {
    int d;
    size_t need = (lp > lr ? lp : lr) + 4;
    ensure_cap(need);
    mul_add_small(R, &lr, 9, 0);
    memcpy(T1, P, lp * sizeof(uint32_t));
    l1 = lp;
    mul_add_small(T1, &l1, 6, 1);          /* 6p + 1 */
    memcpy(T2, T1, l1 * sizeof(uint32_t));
    l2 = l1;
    mul_add_small(T2, &l2, 2, 2);          /* 12p + 4 */
    if (cmp_big(R, lr, T2, l2) >= 0) { sub_big(R, &lr, T2, l2); d = 2; }
    else if (cmp_big(R, lr, T1, l1) >= 0) { sub_big(R, &lr, T1, l1); d = 1; }
    else d = 0;
    mul_add_small(P, &lp, 3, (uint32_t)d);
    if (nd + 1 >= dcap) {
        idx_t nc = dcap * 2 + 64;
        dig = (unsigned char *)realloc(dig, (size_t)nc);
        dcap = nc;
    }
    dig[++nd] = (unsigned char)d;
}

seq_t a(idx_t n) {
    if (nd == 0) {
        ensure_cap(16);
        P[0] = 2; lp = 1;
        R[0] = 2; lr = 1;
        dcap = 64;
        dig = (unsigned char *)malloc((size_t)dcap);
        dig[1] = 2;
        nd = 1;
    }
    while (nd < n) step();
    return (seq_t)dig[n];
}
