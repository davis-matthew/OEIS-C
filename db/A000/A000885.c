/*
 * A000885  Number of twin prime pairs < square of n-th prime.
 * https://oeis.org/A000885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{twin prime pairs (p, p+2) with p+2 < prime(n)^2};
 *            computed by a segmented sieve over the 15 twin-pair residue
 *            classes mod 210 (pre-sieved by 11,13,17,19)
 * Range:     u8 n <= 30, u16 n <= 474, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   t885_cnt, t885_inv, t885_isqrt, t885_kcut
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/t885_cnt.h"
#include "../utils/t885_inv.h"
#include "../utils/t885_isqrt.h"
#include "../utils/t885_kcut.h"

#define T885_L ((uint64_t)1 << 18)
#define T885_M ((uint64_t)46189) /* 11*13*17*19 */

static uint32_t *t885_P = NULL;
static size_t t885_NP = 0;
static uint64_t t885_lim = 0;
static uint64_t *t885_A = NULL;
static size_t t885_Acap = 0;
static size_t t885_next = 3;
static uint64_t t885_kb = 0, t885_tot = 0;
static int t885_ready = 0;
static int t885_c[15];
static int t885_nc = 0;
static unsigned char *t885_pat = NULL;
static unsigned char *t885_seg = NULL;
static uint64_t *t885_nx[15];
static size_t t885_nact = 0, t885_ncap = 0;
static size_t *t885_tm = NULL;
static uint64_t *t885_acc = NULL;
static size_t t885_tcap = 0;

static void t885_primes(uint64_t need) {
    if (need <= t885_lim) return;
    uint64_t lim = t885_lim * 2;
    if (lim < need) lim = need;
    if (lim < 1000) lim = 1000;
    char *s = (char *)malloc(lim + 1);
    memset(s, 1, lim + 1);
    s[0] = s[1] = 0;
    for (uint64_t i = 2; i * i <= lim; i++)
        if (s[i])
            for (uint64_t j = i * i; j <= lim; j += i) s[j] = 0;
    size_t cnt = 0;
    for (uint64_t i = 2; i <= lim; i++) if (s[i]) cnt++;
    free(t885_P);
    t885_P = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    size_t k = 0;
    for (uint64_t i = 2; i <= lim; i++) if (s[i]) t885_P[k++] = (uint32_t)i;
    t885_NP = cnt;
    t885_lim = lim;
    free(s);
}

static void t885_init(void) {
    for (int c = 1; c < 210; c++) {
        bool ok = true;
        int pr[3] = {2, 3, 5};
        for (int i = 0; i < 3; i++)
            if (c % pr[i] == 0 || (c + 2) % pr[i] == 0) ok = false;
        if (c % 7 == 0 || (c + 2) % 7 == 0) ok = false;
        if (ok) t885_c[t885_nc++] = c;
    }
    t885_pat = (unsigned char *)malloc(15 * T885_M);
    for (int ci = 0; ci < t885_nc; ci++) {
        for (uint64_t j = 0; j < T885_M; j++) {
            uint64_t v = 210 * j + (uint64_t)t885_c[ci];
            bool ok = (v % 11) && (v % 13) && (v % 17) && (v % 19) &&
                      ((v + 2) % 11) && ((v + 2) % 13) && ((v + 2) % 17) && ((v + 2) % 19);
            t885_pat[ci * T885_M + j] = ok ? 1 : 0;
        }
    }
    t885_seg = (unsigned char *)malloc(T885_L);
    for (int ci = 0; ci < 15; ci++) t885_nx[ci] = NULL;
    t885_Acap = 1024;
    t885_A = (uint64_t *)malloc(t885_Acap * sizeof(uint64_t));
    t885_A[1] = 0;
    t885_A[2] = 2;
    t885_next = 3;
    t885_ready = 1;
}

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;
    if (!t885_ready) t885_init();
    while ((uint64_t)t885_next <= (uint64_t)n) {
        uint64_t kb = t885_kb, kend = kb + T885_L;
        uint64_t hi = 210 * kend + 1000;
        t885_primes(2 * t885_isqrt(hi) + 10);
        if (t885_NP + 1 > t885_Acap) {
            size_t nc = t885_Acap;
            while (nc < t885_NP + 1) nc *= 2;
            t885_A = (uint64_t *)realloc(t885_A, nc * sizeof(uint64_t));
            t885_Acap = nc;
        }
        /* thresholds finalized in this segment */
        size_t nt = 0;
        size_t m = t885_next;
        while (m - 1 < t885_NP) {
            uint64_t p = t885_P[m - 1];
            if (t885_kcut(p * p, 11) > kend) break;
            if (nt >= t885_tcap) {
                size_t nc = t885_tcap ? t885_tcap * 2 : 256;
                t885_tm = (size_t *)realloc(t885_tm, nc * sizeof(size_t));
                t885_acc = (uint64_t *)realloc(t885_acc, nc * sizeof(uint64_t));
                t885_tcap = nc;
            }
            t885_tm[nt] = m;
            t885_acc[nt] = 0;
            nt++;
            m++;
        }
        /* activate new sieving primes (from 23 on, index 8) */
        uint64_t top = 210 * (kend - 1) + 211;
        while (8 + t885_nact < t885_NP) {
            uint64_t q = t885_P[8 + t885_nact];
            if (q * q > top) break;
            if (t885_nact >= t885_ncap) {
                size_t nc = t885_ncap ? t885_ncap * 2 : 1024;
                for (int ci = 0; ci < t885_nc; ci++)
                    t885_nx[ci] = (uint64_t *)realloc(t885_nx[ci], 2 * nc * sizeof(uint64_t));
                t885_ncap = nc;
            }
            uint64_t iv = t885_inv(210 % q, q);
            for (int ci = 0; ci < t885_nc; ci++) {
                for (int t = 0; t < 2; t++) {
                    uint64_t off = (uint64_t)t885_c[ci] + 2 * (uint64_t)t;
                    uint64_t r = ((q - off % q) % q) * iv % q;
                    uint64_t k0 = kb + ((r + q - kb % q) % q);
                    if (210 * k0 + off == q) k0 += q;
                    t885_nx[ci][2 * t885_nact + t] = k0;
                }
            }
            t885_nact++;
        }
        uint64_t segtot = 0;
        unsigned char *s = t885_seg;
        for (int ci = 0; ci < t885_nc; ci++) {
            uint64_t c = (uint64_t)t885_c[ci];
            uint64_t off = kb % T885_M, pos = 0;
            while (pos < T885_L) {
                uint64_t len = T885_M - off;
                if (len > T885_L - pos) len = T885_L - pos;
                memcpy(s + pos, t885_pat + ci * T885_M + off, len);
                pos += len;
                off = 0;
            }
            uint64_t *nx = t885_nx[ci];
            for (size_t j = 0; j < t885_nact; j++) {
                uint64_t q = t885_P[8 + j];
                for (int t = 0; t < 2; t++) {
                    uint64_t i = nx[2 * j + t] - kb;
                    while (i < T885_L) { s[i] = 0; i += q; }
                    nx[2 * j + t] = kb + i;
                }
            }
            if (kb == 0 && (c == 11 || c == 17)) s[0] = 1;
            uint64_t r = 0;
            size_t p0 = 0;
            for (size_t j = 0; j < nt; j++) {
                uint64_t pm = t885_P[t885_tm[j] - 1];
                uint64_t cut = t885_kcut(pm * pm, c);
                size_t cu = (size_t)(cut - kb);
                r += t885_cnt(s, p0, cu);
                p0 = cu;
                t885_acc[j] += r;
            }
            r += t885_cnt(s, p0, (size_t)T885_L);
            segtot += r;
        }
        for (size_t j = 0; j < nt; j++)
            t885_A[t885_tm[j]] = 2 + t885_tot + t885_acc[j];
        t885_tot += segtot;
        t885_kb = kend;
        t885_next = m;
    }
    return (seq_t)t885_A[n];
}
