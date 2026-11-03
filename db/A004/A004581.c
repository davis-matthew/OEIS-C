/*
 * A004581  Expansion of sqrt(8) in base 6.
 * https://oeis.org/A004581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   a(n) = floor(sqrt(8) * 6^(n-1)) mod 6, i.e. a(n) =
 *            floor(sqrt(8*36^(n-1))) mod 6; computed by the digit-by-digit
 *            (schoolbook) square-root algorithm in base 6 with private
 *            multi-limb integers, memoized across calls
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 115 terms through n = 9671 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int A004581_cmp(const uint32_t *a, size_t al, const uint32_t *b, size_t bl) {
    size_t i;
    if (al != bl) return al < bl ? -1 : 1;
    i = al;
    while (i--) {
        if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
    }
    return 0;
}

/* dst = src*m + add (dst may alias src); returns normalized length */
static size_t A004581_mulsm(uint32_t *dst, const uint32_t *src, size_t len,
                            uint32_t m, uint32_t add) {
    uint64_t c = add;
    size_t i;
    for (i = 0; i < len; i++) {
        c += (uint64_t)src[i] * m;
        dst[i] = (uint32_t)c;
        c >>= 32;
    }
    if (c) dst[len++] = (uint32_t)c;
    while (len && dst[len - 1] == 0) len--;
    return len;
}

/* a -= b (requires a >= b); returns normalized length */
static size_t A004581_sub(uint32_t *a, size_t al, const uint32_t *b, size_t bl) {
    int64_t borrow = 0;
    size_t i;
    for (i = 0; i < al; i++) {
        int64_t t = (int64_t)a[i] - borrow - (i < bl ? (int64_t)b[i] : 0);
        if (t < 0) { t += ((int64_t)1 << 32); borrow = 1; } else borrow = 0;
        a[i] = (uint32_t)t;
    }
    while (al && a[al - 1] == 0) al--;
    return al;
}

static unsigned char *A004581_dig = 0;
static size_t A004581_ndig = 0, A004581_digcap = 0;
static uint32_t *A004581_R = 0, *A004581_P = 0, *A004581_Q = 0, *A004581_V = 0;
static size_t A004581_rl = 0, A004581_pl = 0, A004581_lcap = 0;

static void A004581_ensure(size_t k) {
    size_t need;
    if (k <= A004581_digcap) return;
    {
        size_t nc = A004581_digcap * 2;
        if (nc < k) nc = k;
        if (nc < 64) nc = 64;
        A004581_dig = (unsigned char *)realloc(A004581_dig, nc);
        A004581_digcap = nc;
    }
    need = A004581_digcap / 12 + 8;
    if (need > A004581_lcap) {
        A004581_R = (uint32_t *)realloc(A004581_R, need * sizeof(uint32_t));
        A004581_P = (uint32_t *)realloc(A004581_P, need * sizeof(uint32_t));
        A004581_Q = (uint32_t *)realloc(A004581_Q, need * sizeof(uint32_t));
        A004581_V = (uint32_t *)realloc(A004581_V, need * sizeof(uint32_t));
        A004581_lcap = need;
    }
}

static void A004581_step(void) {
    /* bring down next pair of base-6 digits: 8 = "12"_6 first, then "00" */
    uint32_t pair = (A004581_ndig == 0) ? 8u : 0u;
    size_t ql, vl = 0;
    uint32_t x;
    A004581_rl = A004581_mulsm(A004581_R, A004581_R, A004581_rl, 36, pair);
    ql = A004581_mulsm(A004581_Q, A004581_P, A004581_pl, 12, 0);
    for (x = 5; x > 0; x--) {
        vl = A004581_mulsm(A004581_V, A004581_Q, ql, x, x * x);
        if (A004581_cmp(A004581_V, vl, A004581_R, A004581_rl) <= 0) break;
    }
    if (x > 0)
        A004581_rl = A004581_sub(A004581_R, A004581_rl, A004581_V, vl);
    A004581_pl = A004581_mulsm(A004581_P, A004581_P, A004581_pl, 6, x);
    A004581_dig[A004581_ndig++] = (unsigned char)x;
}

seq_t a(idx_t n) {
    size_t k = (size_t)n;
    if (n < 1) return (seq_t)0;
    A004581_ensure(k);
    while (A004581_ndig < k) A004581_step();
    return (seq_t)A004581_dig[k - 1];
}
