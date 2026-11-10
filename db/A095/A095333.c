/*
 * A095333  Number of A095313-primes in range [2^n,2^(n+1)].
 * https://oeis.org/A095333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ primes p : 2^n < p < 2^(n+1), 2*A000120(p) <= n+7 }.
 *            Computed with a segmented mod-30 wheel sieve of Eratosthenes.
 *            Primes up to 103 are removed with precomputed patterns. Primes
 *            are counted in blocks of 3840 numbers, where the popcount of the
 *            high part is fixed and the low byte is looke...
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Validated: partial, pass, 33 terms through n = 33 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static const unsigned char A095333_R[8] = {1, 7, 11, 13, 17, 19, 23, 29};

#define A095333_NGRP 7
static const uint32_t A095333_grp[A095333_NGRP][5] = {
    {7, 11, 13, 17, 19}, {23, 29, 31, 37, 0}, {41, 43, 47, 0, 0},
    {53, 59, 61, 0, 0}, {67, 71, 73, 0, 0}, {79, 83, 89, 0, 0},
    {97, 101, 103, 0, 0}};
static unsigned char *A095333_pat[A095333_NGRP];
static size_t A095333_per[A095333_NGRP];
static unsigned char A095333_TA[128][10], A095333_TB[128][10], A095333_KB[128];
static unsigned char A095333_PC8[256];
static bool A095333_init_done = false;

static inline unsigned A095333_pc64(uint64_t x)
{
    x = x - ((x >> 1) & 0x5555555555555555ULL);
    x = (x & 0x3333333333333333ULL) + ((x >> 2) & 0x3333333333333333ULL);
    x = (x + (x >> 4)) & 0x0f0f0f0f0f0f0f0fULL;
    return (unsigned)((x * 0x0101010101010101ULL) >> 56);
}

static void A095333_init(void)
{
    if (A095333_init_done) return;
    for (int g = 0; g < A095333_NGRP; g++) {
        size_t per = 1;
        for (int k = 0; k < 5 && A095333_grp[g][k]; k++) per *= A095333_grp[g][k];
        unsigned char *pat = (unsigned char *)calloc(per, 1);
        if (!pat) exit(1);
        for (int k = 0; k < 5 && A095333_grp[g][k]; k++) {
            uint32_t p = A095333_grp[g][k];
            for (int j = 0; j < 8; j++) {
                uint32_t t0 = 0;
                while ((30u * t0 + A095333_R[j]) % p != 0) t0++;
                for (size_t t = t0; t < per; t += p) pat[t] |= (unsigned char)(1u << j);
            }
        }
        A095333_pat[g] = pat;
        A095333_per[g] = per;
    }
    for (int i = 0; i < 256; i++) A095333_PC8[i] = (unsigned char)A095333_pc64((uint64_t)i);
    for (int o = 0; o < 128; o++) {
        unsigned kb = (unsigned)(30 * o) >> 8;
        A095333_KB[o] = (unsigned char)kb;
        for (int th = 0; th < 10; th++) { A095333_TA[o][th] = 0; A095333_TB[o][th] = 0; }
        for (int j = 0; j < 8; j++) {
            unsigned y = 30u * (unsigned)o + A095333_R[j];
            unsigned k = y >> 8, pl = A095333_PC8[y & 255];
            for (int th = 0; th < 10; th++) {
                if ((int)pl <= th - 1) {
                    if (k == kb) A095333_TA[o][th] |= (unsigned char)(1u << j);
                    else A095333_TB[o][th] |= (unsigned char)(1u << j);
                }
            }
        }
    }
    A095333_init_done = true;
}

static uint64_t A095333_small(unsigned n)
{
    uint64_t lo = 1ULL << n, hi = 1ULL << (n + 1);
    unsigned char *c = (unsigned char *)calloc((size_t)hi, 1);
    if (!c) exit(1);
    for (uint64_t i = 2; i * i < hi; i++)
        if (!c[i])
            for (uint64_t j = i * i; j < hi; j += i) c[j] = 1;
    uint64_t cnt = 0;
    for (uint64_t v = lo + 1; v < hi; v++)
        if (!c[v] && 2 * A095333_pc64(v) <= n + 7) cnt++;
    free(c);
    return cnt;
}

static void A095333_apply(unsigned char *seg, size_t len, const unsigned char *pat,
                          size_t period, size_t off, int first)
{
    size_t done = 0;
    while (done < len) {
        size_t ch = period - off;
        if (ch > len - done) ch = len - done;
        if (first) memcpy(seg + done, pat + off, ch);
        else {
            const unsigned char *src = pat + off;
            unsigned char *dst = seg + done;
            for (size_t x = 0; x < ch; x++) dst[x] |= src[x];
        }
        done += ch; off = 0;
    }
}

static uint64_t A095333_big(unsigned n)
{
    const uint64_t lo = 1ULL << n, hi = 1ULL << (n + 1);
    const unsigned P = (n + 7) / 2;
    A095333_init();

    int idx[30];
    for (int i = 0; i < 30; i++) idx[i] = -1;
    for (int j = 0; j < 8; j++) idx[A095333_R[j]] = j;
    unsigned char dfrac[30][8], mtab[30][8];
    for (int j = 0; j < 8; j++) {
        int cls = A095333_R[j];
        for (int w = 0; w < 8; w++) {
            int prod = cls * A095333_R[w];
            dfrac[cls][w] = (unsigned char)(prod / 30);
            mtab[cls][w] = (unsigned char)(1u << idx[prod % 30]);
        }
    }

    uint64_t r = 1ULL << ((n + 1) / 2);
    while (r * r > hi - 1) r--;
    while ((r + 1) * (r + 1) <= hi - 1) r++;
    unsigned char *sm = (unsigned char *)calloc((size_t)r + 1, 1);
    if (!sm) exit(1);
    size_t cap = (size_t)r / 2 + 2, np = 0;
    uint32_t *pp = (uint32_t *)malloc(sizeof(uint32_t) * cap);
    int64_t *cb = (int64_t *)malloc(sizeof(int64_t) * cap);
    int64_t *nx = (int64_t *)malloc(sizeof(int64_t) * cap);
    unsigned char *wi = (unsigned char *)malloc(cap);
    if (!pp || !cb || !nx || !wi) exit(1);

    const uint64_t B0 = (lo / 30) & ~(uint64_t)127;
    int64_t NB = (int64_t)((hi - 1) / 30 - B0 + 1);
    NB = (NB + 127) & ~(int64_t)127;

    for (uint64_t i = 2; i <= r; i++) {
        if (sm[i]) continue;
        for (uint64_t j = i * i; j <= r; j += i) sm[j] = 1;
        if (i < 107) continue;
        uint64_t p = i;
        uint64_t q0 = (lo + p - 1) / p;
        uint64_t k = q0 / 30, rem = q0 % 30;
        int w = 0;
        while (w < 8 && A095333_R[w] < rem) w++;
        if (w == 8) { k++; w = 0; }
        int64_t c = (int64_t)(p * k) - (int64_t)B0;
        unsigned cls = (unsigned)(p % 30);
        pp[np] = (uint32_t)p;
        cb[np] = c;
        wi[np] = (unsigned char)w;
        nx[np] = c + (int64_t)((p / 30) * A095333_R[w] + dfrac[cls][w]);
        np++;
    }
    free(sm);

    const int64_t S = 131072;
    unsigned char *seg = (unsigned char *)malloc((size_t)S);
    if (!seg) exit(1);
    uint64_t count = 0;

    for (int64_t ss = 0; ss < NB; ss += S) {
        int64_t len = (NB - ss < S) ? NB - ss : S;
        int64_t se = ss + len;
        uint64_t g = B0 + (uint64_t)ss;

        for (int gi = 0; gi < A095333_NGRP; gi++)
            A095333_apply(seg, (size_t)len, A095333_pat[gi], A095333_per[gi],
                          (size_t)(g % A095333_per[gi]), gi == 0);

        for (size_t i = 0; i < np; i++) {
            if (nx[i] >= se) continue;
            int64_t p = pp[i];
            int64_t q = p / 30;
            unsigned cls = (unsigned)(p % 30);
            int64_t d[8];
            unsigned char m[8];
            for (int w = 0; w < 8; w++) {
                d[w] = q * A095333_R[w] + dfrac[cls][w];
                m[w] = mtab[cls][w];
            }
            int64_t c = cb[i];
            int w = wi[i];
            bool stop = false;
            while (w < 8) {
                int64_t o = c + d[w];
                if (o >= se) { stop = true; break; }
                seg[o - ss] |= m[w];
                w++;
            }
            if (!stop) {
                c += p; w = 0;
                int64_t lim7 = se - d[7];
                while (c < lim7) {
                    unsigned char *bp = seg + (c - ss);
                    bp[d[0]] |= m[0]; bp[d[1]] |= m[1];
                    bp[d[2]] |= m[2]; bp[d[3]] |= m[3];
                    bp[d[4]] |= m[4]; bp[d[5]] |= m[5];
                    bp[d[6]] |= m[6]; bp[d[7]] |= m[7];
                    c += p;
                }
                for (;;) {
                    int64_t o = c + d[w];
                    if (o >= se) break;
                    seg[o - ss] |= m[w];
                    w++;
                }
            }
            cb[i] = c;
            wi[i] = (unsigned char)w;
            nx[i] = c + d[w];
        }

        for (int64_t i = 0; i < len; i += 128) {
            uint64_t t0 = g + (uint64_t)i;
            const unsigned char *b = seg + i;
            uint64_t vlo = 30ULL * t0, vhi = 30ULL * (t0 + 128);
            if (vlo > lo && vhi <= hi) {
                uint64_t U = (t0 / 128) * 15;
                unsigned pcs[16], mn = 1000, mx = 0;
                for (int k = 0; k < 16; k++) {
                    pcs[k] = A095333_pc64(U + (uint64_t)k);
                    if (pcs[k] < mn) mn = pcs[k];
                    if (pcs[k] > mx) mx = pcs[k];
                }
                if (mn > P) continue;
                if (mx + 8 <= P) {
                    for (int w = 0; w < 16; w++) {
                        uint64_t x;
                        memcpy(&x, b + 8 * w, 8);
                        count += A095333_pc64(~x);
                    }
                    continue;
                }
                unsigned char thi[16];
                for (int k = 0; k < 16; k++) {
                    int th = (int)P - (int)pcs[k] + 1;
                    if (th < 0) th = 0;
                    if (th > 9) th = 9;
                    thi[k] = (unsigned char)th;
                }
                for (int o = 0; o < 128; o++) {
                    unsigned kb = A095333_KB[o];
                    unsigned char mask = (unsigned char)(A095333_TA[o][thi[kb]] |
                                                         A095333_TB[o][thi[kb + 1]]);
                    count += A095333_PC8[(unsigned char)(~b[o]) & mask];
                }
            } else {
                for (int o = 0; o < 128; o++) {
                    unsigned u = (unsigned char)~b[o];
                    while (u) {
                        int j = __builtin_ctz(u);
                        u &= u - 1;
                        uint64_t v = 30ULL * (t0 + (uint64_t)o) + A095333_R[j];
                        if (v > lo && v < hi && A095333_pc64(v) <= P) count++;
                    }
                }
            }
        }
    }

    free(seg); free(pp); free(cb); free(nx); free(wi);
    return count;
}

seq_t a(idx_t n)
{
    static uint64_t memo[64];
    static bool have[64];
    if (n == 0 || n >= 62) return 0;
    if (have[n]) return (seq_t)memo[n];
    uint64_t v = (n < 16) ? A095333_small((unsigned)n) : A095333_big((unsigned)n);
    memo[n] = v;
    have[n] = true;
    return (seq_t)v;
}
