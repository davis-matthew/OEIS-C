/*
 * A095330  Number of A095320-primes in range ]2^n,2^(n+1)].
 * https://oeis.org/A095330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{primes p : 2^n < p <= 2^(n+1), popcount(p) >
 *            (n+1-popcount(p)) - 3}, i.e. popcount(p) >= floor(n/2); computed
 *            by a segmented mod-30 wheel sieve with pattern presieving and
 *            table-driven popcount counting
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Validated: partial, pass, 33 terms through n = 33 (2026-11-07)
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/isqrt_u64.h"

static const unsigned char A095330_W[8] = {1, 7, 11, 13, 17, 19, 23, 29};
static unsigned char A095330_t16[65536];
static unsigned char A095330_pop8[256];
static unsigned char A095330_ctz8[256];
static unsigned char *A095330_M = NULL;   /* 17 rows x 65536 masks */
static bool A095330_tinit = false;

#define A095330_PATLIM 320u
#define A095330_MAXPAT 64
static unsigned char *A095330_pat[A095330_MAXPAT];
static size_t A095330_per[A095330_MAXPAT];
static int A095330_npat = 0;
static uint64_t A095330_firstcross = 0;   /* first prime not covered by patterns */

static unsigned A095330_pop64(uint64_t x)
{
    unsigned c = 0;
    while (x) { c += A095330_t16[x & 0xFFFF]; x >>= 16; }
    return c;
}

static void A095330_build_pattern(const unsigned *pr, int cnt)
{
    size_t per = 1;
    for (int i = 0; i < cnt; i++) per *= pr[i];
    unsigned char *pat = (unsigned char *)calloc(per, 1);
    if (!pat) abort();
    for (int i = 0; i < cnt; i++) {
        uint64_t p = pr[i];
        for (int b = 0; b < 8; b++) {
            uint64_t i0 = 0;
            while ((30 * i0 + A095330_W[b]) % p != 0) i0++;
            for (uint64_t j = i0; j < per; j += p)
                pat[j] |= (unsigned char)(1u << b);
        }
    }
    A095330_pat[A095330_npat] = pat;
    A095330_per[A095330_npat] = per;
    A095330_npat++;
}

static void A095330_init(void)
{
    if (A095330_tinit) return;
    A095330_t16[0] = 0;
    for (unsigned i = 1; i < 65536; i++)
        A095330_t16[i] = (unsigned char)(A095330_t16[i >> 1] + (i & 1));
    for (unsigned i = 0; i < 256; i++) A095330_pop8[i] = A095330_t16[i];
    A095330_ctz8[0] = 8;
    for (unsigned i = 1; i < 256; i++) {
        unsigned c = 0, v = i;
        while (!(v & 1)) { v >>= 1; c++; }
        A095330_ctz8[i] = (unsigned char)c;
    }
    A095330_M = (unsigned char *)malloc((size_t)17 * 65536);
    if (!A095330_M) abort();
    for (unsigned k = 0; k < 17; k++) {
        unsigned char *row = A095330_M + (size_t)k * 65536;
        for (unsigned lb = 0; lb < 65536; lb++) {
            unsigned char m = 0;
            for (int b = 0; b < 8; b++) {
                unsigned y = lb + A095330_W[b];
                if (y < 65536 && A095330_t16[y] >= k) m |= (unsigned char)(1u << b);
            }
            row[lb] = m;
        }
    }
    /* patterns */
    {
        unsigned g0[5] = {7, 11, 13, 17, 19};
        A095330_build_pattern(g0, 5);
        unsigned sp[128];
        int ns = 0;
        for (unsigned x = 23; x <= A095330_PATLIM; x += 2) {
            bool pr = true;
            for (unsigned d = 3; d * d <= x; d += 2)
                if (x % d == 0) { pr = false; break; }
            if (pr) sp[ns++] = x;
        }
        for (int i = 0; i < ns; i += 2) {
            unsigned pair[2];
            pair[0] = sp[i];
            if (i + 1 < ns) { pair[1] = sp[i + 1]; A095330_build_pattern(pair, 2); }
            else A095330_build_pattern(pair, 1);
        }
        A095330_firstcross = (uint64_t)sp[ns - 1] + 1;
    }
    A095330_tinit = true;
}

static bool A095330_isprime_small(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

static void A095330_fill(unsigned char *seg, uint64_t len, int pi, uint64_t start)
{
    const unsigned char *pat = A095330_pat[pi];
    const size_t per = A095330_per[pi];
    size_t o = (size_t)(start % per);
    uint64_t pos = 0;
    while (pos < len) {
        size_t c = per - o;
        if ((uint64_t)c > len - pos) c = (size_t)(len - pos);
        if (pi == 0) memcpy(seg + pos, pat + o, c);
        else {
            unsigned char *d = seg + pos;
            const unsigned char *s = pat + o;
            for (size_t t = 0; t < c; t++) d[t] |= s[t];
        }
        pos += c;
        o = 0;
    }
}

static uint64_t A095330_count(unsigned n)
{
    const uint64_t lo = (uint64_t)1 << n;
    const uint64_t hi = lo << 1;
    const int K = (int)(n / 2);
    uint64_t cnt = 0;

    if (n < 12) {
        for (uint64_t x = lo + 1; x <= hi; x++)
            if (A095330_isprime_small(x) && (int)A095330_pop64(x) >= K) cnt++;
        return cnt;
    }

    /* crossing primes: firstcross..r */
    uint64_t r = isqrt_u64(hi);
    size_t np = 0;
    uint64_t *P = NULL;
    if (r >= A095330_firstcross) {
        unsigned char *comp = (unsigned char *)calloc((size_t)r + 1, 1);
        if (!comp) abort();
        for (uint64_t i = 2; i <= r; i++) {
            if (comp[i]) continue;
            if (i >= A095330_firstcross) np++;
            for (uint64_t j = i * i; j <= r; j += i) comp[j] = 1;
        }
        P = (uint64_t *)malloc(sizeof(uint64_t) * (np + 1));
        if (!P) abort();
        size_t k = 0;
        for (uint64_t i = A095330_firstcross; i <= r; i++)
            if (!comp[i]) P[k++] = i;
        free(comp);
    }
    uint64_t *off = (uint64_t *)malloc(sizeof(uint64_t) * (np * 8 + 1));
    unsigned char *msk = (unsigned char *)malloc(np * 8 + 1);
    if (!off || !msk) abort();

    int idx30[30];
    for (int i = 0; i < 30; i++) idx30[i] = -1;
    for (int b = 0; b < 8; b++) idx30[A095330_W[b]] = b;

    const uint64_t g0 = lo / 30, g1 = hi / 30;
    const uint64_t nbytes = g1 - g0 + 1;
    const uint64_t S = 30 * g0;

    for (size_t i = 0; i < np; i++) {
        uint64_t p = P[i];
        uint64_t q0 = (S + p - 1) / p;
        for (int k = 0; k < 8; k++) {
            uint64_t d = (A095330_W[k] + 30 - q0 % 30) % 30;
            uint64_t q = q0 + d;
            uint64_t m = p * q;
            off[i * 8 + k] = m / 30 - g0;
            msk[i * 8 + k] = (unsigned char)(1u << idx30[m % 30]);
        }
    }

    const uint64_t SEG = (uint64_t)1 << 18;
    unsigned char *seg = (unsigned char *)malloc((size_t)SEG);
    if (!seg) abort();

    uint64_t curh = UINT64_MAX;
    const unsigned char *row = NULL;

    for (uint64_t gb = 0; gb < nbytes; gb += SEG) {
        uint64_t len = nbytes - gb;
        if (len > SEG) len = SEG;

        for (int pi = 0; pi < A095330_npat; pi++)
            A095330_fill(seg, len, pi, g0 + gb);

        for (size_t i = 0; i < np; i++) {
            const uint64_t p = P[i];
            uint64_t *of = off + i * 8;
            const unsigned char *mk = msk + i * 8;
            for (int k = 0; k < 8; k++) {
                uint64_t j = of[k];
                const unsigned char m = mk[k];
                while (j + 3 * p < len) {
                    seg[j] |= m;
                    seg[j + p] |= m;
                    seg[j + 2 * p] |= m;
                    seg[j + 3 * p] |= m;
                    j += 4 * p;
                }
                while (j < len) { seg[j] |= m; j += p; }
                of[k] = j - len;
            }
        }

        uint64_t g = g0 + gb;
        uint64_t base = 30 * g;
        for (uint64_t i = 0; i < len; i++, g++, base += 30) {
            unsigned v = (unsigned)(~seg[i]) & 0xFFu;
            if (!v) continue;
            unsigned lb = (unsigned)(base & 0xFFFF);
            if (lb > 65535u - 29u || g == g0 || g == g1) {
                while (v) {
                    unsigned b = A095330_ctz8[v];
                    v &= v - 1;
                    uint64_t x = base + A095330_W[b];
                    if (x <= lo || x > hi) continue;
                    if ((int)A095330_pop64(x) >= K) cnt++;
                }
                continue;
            }
            uint64_t h = base >> 16;
            if (h != curh) {
                curh = h;
                int need = K - (int)A095330_pop64(h);
                if (need <= 0) row = A095330_M;
                else if (need > 16) row = NULL;
                else row = A095330_M + (size_t)need * 65536;
            }
            if (row) cnt += A095330_pop8[v & row[lb]];
        }
    }

    free(seg); free(P); free(off); free(msk);
    return cnt;
}

seq_t a(idx_t n)
{
    static uint64_t memo[64];
    static bool have[64];
    if (n < 1 || n > 62) return 0;
    A095330_init();
    if (!have[n]) {
        memo[n] = A095330_count((unsigned)n);
        have[n] = true;
    }
    return (seq_t)memo[n];
}
