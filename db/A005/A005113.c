/*
 * A005113  Smallest prime in class n (sometimes written n+) according to the
 *          Erdős-Selfridge classification of primes.
 * https://oeis.org/A005113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  more, nonn
 * Formula:   search, not a closed form. a(1) = 2. A prime p has class >= k+1
 *            when p+1 = m*q with m even and q a prime >= 5 of class >= k, so
 *            a(k+1) is the smallest prime m*q-1 with q of class >= k. Each
 *            level keeps the smallest class->=k primes and builds the next
 *            level from them, using a heap merge over the...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define KEEP 12000
#define MAXLV 64

typedef unsigned __int128 a5113_u128;

static uint64_t mont_n, mont_ninv;

static uint64_t mredc(a5113_u128 t) {
    uint64_t lo = (uint64_t)t, hi = (uint64_t)(t >> 64);
    uint64_t m = lo * mont_ninv;
    uint64_t mh = (uint64_t)(((a5113_u128)m * mont_n) >> 64);
    uint64_t r = hi - mh;
    if (hi < mh) r += mont_n;
    return r;
}

static uint64_t mmul(uint64_t a, uint64_t b) {
    return mredc((a5113_u128)a * b);
}

static const uint64_t small_pr[] = {3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53};

static bool mr(uint64_t n) {
    static const uint64_t b1[] = {2, 3, 5, 7};
    static const uint64_t b2[] = {2, 3, 5, 7, 11, 13, 17};
    static const uint64_t b3[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    const uint64_t *bases;
    int nb, i, j, s;
    uint64_t inv, r1, r2, one, mone, d;
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (i = 0; i < 15; i++) {
        if (n == small_pr[i]) return true;
        if (n % small_pr[i] == 0) return false;
    }
    if (n < 59 * 59) return true;
    if (n < 3215031751ULL) { bases = b1; nb = 4; }
    else if (n < 341550071728321ULL) { bases = b2; nb = 7; }
    else { bases = b3; nb = 7; }
    mont_n = n;
    inv = n;
    for (i = 0; i < 5; i++) inv *= 2 - n * inv;
    mont_ninv = inv;
    r1 = (uint64_t)((((a5113_u128)1) << 64) % n);
    r2 = (uint64_t)(((a5113_u128)r1 * r1) % n);
    one = r1;
    mone = n - r1;
    d = n - 1;
    s = 0;
    while ((d & 1) == 0) { d >>= 1; s++; }
    for (i = 0; i < nb; i++) {
        uint64_t a = bases[i] % n, x, b, e;
        bool ok = false;
        if (a == 0) continue;
        b = mmul(a, r2);
        x = one;
        e = d;
        while (e) {
            if (e & 1) x = mmul(x, b);
            b = mmul(b, b);
            e >>= 1;
        }
        if (x == one || x == mone) continue;
        for (j = 1; j < s; j++) {
            x = mmul(x, x);
            if (x == mone) { ok = true; break; }
        }
        if (!ok) return false;
    }
    return true;
}

static uint64_t *cur = NULL;
static size_t cur_len = 0;
static int lv_done = 0;
static uint64_t vals[MAXLV + 2];

static void init_lv1(void) {
    size_t c = 0;
    uint64_t p;
    cur = (uint64_t *)malloc(KEEP * sizeof(uint64_t));
    for (p = 5; c < KEEP; p += 2)
        if (mr(p)) cur[c++] = p;
    cur_len = c;
    vals[1] = 2;
    lv_done = 1;
}

static bool next_level(void) {
    uint64_t *hv = (uint64_t *)malloc(cur_len * sizeof(uint64_t));
    uint64_t *hs = (uint64_t *)malloc(cur_len * sizeof(uint64_t));
    uint64_t *out = (uint64_t *)malloc(KEEP * sizeof(uint64_t));
    size_t hn = 0, cnt = 0, i;
    uint64_t last = 0;
    for (i = 0; i < cur_len; i++) {
        if (cur[i] > UINT64_MAX / 2) break;
        hv[hn] = 2 * cur[i] - 1;
        hs[hn] = 2 * cur[i];
        hn++;
    }
    while (cnt < KEEP && hn > 0) {
        uint64_t v = hv[0], s = hs[0];
        size_t k = 0;
        if (v != last) {
            last = v;
            if (mr(v)) out[cnt++] = v;
        }
        if (v <= UINT64_MAX - s) {
            hv[0] = v + s;
        } else {
            hn--;
            if (hn == 0) break;
            hv[0] = hv[hn];
            hs[0] = hs[hn];
        }
        for (;;) {
            size_t l = 2 * k + 1, c;
            uint64_t tv, ts;
            if (l >= hn) break;
            c = l;
            if (l + 1 < hn && hv[l + 1] < hv[l]) c = l + 1;
            if (hv[c] >= hv[k]) break;
            tv = hv[c]; hv[c] = hv[k]; hv[k] = tv;
            ts = hs[c]; hs[c] = hs[k]; hs[k] = ts;
            k = c;
        }
    }
    free(hv);
    free(hs);
    if (cnt == 0) {
        free(out);
        return false;
    }
    free(cur);
    cur = out;
    cur_len = cnt;
    lv_done++;
    vals[lv_done] = out[0];
    return true;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (lv_done == 0) init_lv1();
    while ((idx_t)lv_done < n) {
        if (lv_done >= MAXLV || !next_level()) return 0;
    }
    return (seq_t)vals[n];
}
