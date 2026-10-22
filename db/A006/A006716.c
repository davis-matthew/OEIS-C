/*
 * A006716  Squares with digits 1, 4, 9.
 * https://oeis.org/A006716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search, not a closed form. a(n) is the n-th square whose digits
 *            are all 1, 4 or 9. Small squares are found by brute force over
 *            k. Larger ones use meet-in-the-middle: the leading digits of k^2
 *            (all 1/4/9) fix an interval for k, and k mod 10^12 must make the
 *            last 12 digits of k^2 also 1/4/9
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 10, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   ok64, ok_digit, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/u128_to_seq.h"
#include "../utils/ok64.h"
#include "../utils/ok_digit.h"

typedef unsigned __int128 u128_t;

#define RES_DIGITS 12
#define MAX_D 38

static u128_t P10[MAX_D + 2];
static uint64_t *res = NULL;
static size_t nres = 0;
static u128_t *found = NULL;
static size_t nfound = 0, capfound = 0;
static int nextD = 0;

static void push_found(u128_t v) {
    if (nfound == capfound) {
        capfound = capfound ? capfound * 2 : 64;
        found = (u128_t *)realloc(found, capfound * sizeof(u128_t));
    }
    found[nfound++] = v;
}

static bool ok128(u128_t x) {
    const uint64_t T19 = 10000000000000000000ULL;
    uint64_t hi = (uint64_t)(x / T19);
    uint64_t lo = (uint64_t)(x - (u128_t)hi * T19);
    int i;
    if (hi == 0) return ok64(lo);
    for (i = 0; i < 19; i++) {
        if (!ok_digit((unsigned)(lo % 10))) return false;
        lo /= 10;
    }
    return ok64(hi);
}

static uint64_t isqrt64(uint64_t x) {
    uint64_t r, y, t = x;
    int s = 0;
    if (x == 0) return 0;
    while (t) { s++; t >>= 1; }
    r = 1ULL << ((s + 1) / 2);
    y = (r + x / r) >> 1;
    while (y < r) { r = y; y = (r + x / r) >> 1; }
    return r;
}

static uint64_t isqrt128(u128_t x) {
    uint64_t r, h;
    int b = 64, s;
    if ((x >> 64) == 0) return isqrt64((uint64_t)x);
    h = (uint64_t)(x >> 64);
    while (h) { b++; h >>= 1; }
    s = b - 62;
    if (s & 1) s++;
    r = isqrt64((uint64_t)(x >> s)) << (s / 2);
    if (r == 0) r = 1;
    r = (uint64_t)(((u128_t)r + x / r) >> 1);
    while ((u128_t)r * r > x) r--;
    while ((u128_t)(r + 1) * (r + 1) <= x) r++;
    return r;
}

static uint64_t ceil_sqrt(u128_t x) {
    if (x == 0) return 0;
    return isqrt128(x - 1) + 1;
}

static int cmp_u64(const void *a, const void *b) {
    uint64_t x = *(const uint64_t *)a, y = *(const uint64_t *)b;
    return (x > y) - (x < y);
}

static void build_residues(void) {
    size_t cap = 16, n = 0, i;
    uint64_t *cur = (uint64_t *)malloc(cap * sizeof(uint64_t));
    uint64_t p = 1, k;
    int j;
    for (k = 0; k < 10; k++)
        if (ok_digit((unsigned)(k * k % 10))) cur[n++] = k;
    for (j = 1; j < RES_DIGITS; j++) {
        size_t ncap = n * 10 + 1, nn = 0;
        uint64_t *nxt = (uint64_t *)malloc(ncap * sizeof(uint64_t));
        unsigned d;
        p *= 10;
        for (i = 0; i < n; i++) {
            uint64_t r = cur[i];
            unsigned q = (unsigned)(((u128_t)r * r / p) % 10);
            unsigned c = (unsigned)(r % 10);
            for (d = 0; d < 10; d++)
                if (ok_digit((q + 2 * c * d) % 10)) nxt[nn++] = r + (uint64_t)d * p;
        }
        free(cur);
        cur = nxt;
        n = nn;
    }
    qsort(cur, n, sizeof(uint64_t), cmp_u64);
    res = cur;
    nres = n;
}

static void scan(uint64_t lo, uint64_t hi) {
    const uint64_t M = 1000000000000ULL;
    uint64_t base = lo - lo % M;
    while (base <= hi) {
        uint64_t start = lo > base ? lo - base : 0;
        size_t a0 = 0, b0 = nres, i;
        while (a0 < b0) {
            size_t mid = (a0 + b0) / 2;
            if (res[mid] < start) a0 = mid + 1; else b0 = mid;
        }
        for (i = a0; i < nres; i++) {
            uint64_t k = base + res[i];
            u128_t sq;
            if (k > hi) break;
            sq = (u128_t)k * k;
            if (ok128(sq)) push_found(sq);
        }
        base += M;
    }
}

static void gen(u128_t P, int depth, int m, u128_t shift) {
    static const unsigned dg[3] = {1, 4, 9};
    int i;
    if (depth == m) {
        uint64_t lo = ceil_sqrt(P * shift);
        uint64_t hi = ceil_sqrt((P + 1) * shift) - 1;
        if (lo <= hi) scan(lo, hi);
        return;
    }
    for (i = 0; i < 3; i++) gen(P * 10 + dg[i], depth + 1, m, shift);
}

static void init_all(void) {
    uint64_t k;
    int i;
    P10[0] = 1;
    for (i = 1; i <= MAX_D + 1; i++) P10[i] = P10[i - 1] * 10;
    for (k = 1; k < 1000000ULL; k++)
        if (ok64(k * k)) push_found((u128_t)k * k);
    build_residues();
    nextD = RES_DIGITS + 1;
}

seq_t a(idx_t n) {
    if (nextD == 0) init_all();
    while ((idx_t)nfound < n && nextD <= MAX_D) {
        int D = nextD;
        int m = D / 2 - 7;
        if (m < 1) m = 1;
        gen(0, 0, m, P10[D - m]);
        nextD++;
    }
    if (n == 0 || (idx_t)nfound < n) return (seq_t)0;
    return u128_to_seq(found[n - 1]);
}
