/*
 * A095332  Number of A095312-primes in range ]2^n,2^(n+1)].
 * https://oeis.org/A095332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ primes p : 2^n < p <= 2^(n+1), 2*popcount(p) > (n+1) +
 *            6 }, computed with a segmented wheel-30 sieve of Eratosthenes
 *            over ]2^n, 2^(n+1)] and a table-driven count of the binary
 *            1-bits of each prime found
 * Range:     u8 n <= 15, u16 n <= 22, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Validated: partial, pass, 33 terms through n = 33 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static const unsigned char A095332_R[8] = {1, 7, 11, 13, 17, 19, 23, 29};

#define A095332_PAT 323323u   /* 7*11*13*17*19 wheel-30 bytes */
#define A095332_SEG 65536u    /* segment size in bytes */

static unsigned char *A095332_pat = NULL;
static signed char A095332_ridx[30];
static unsigned char A095332_pm[30][8];
static unsigned char A095332_pop16[65536];
static unsigned char A095332_qt[13][2048];

static unsigned A095332_pop64(uint64_t x)
{
    return (unsigned)A095332_pop16[x & 0xffffu] + A095332_pop16[(x >> 16) & 0xffffu] +
           A095332_pop16[(x >> 32) & 0xffffu] + A095332_pop16[x >> 48];
}

/* Cross off multiples p*(30c+R_i) of p in seg[0..len); cb is the byte index
   (relative to seg) of the current cycle base p*c.  Returns cb for next segment. */
static int64_t A095332_cross(unsigned char *seg, int64_t len, uint64_t p, int64_t cb)
{
    if (cb >= len) return cb - len;
    const unsigned char *mk = A095332_pm[p % 30];
    int64_t d[8];
    for (int i = 0; i < 8; i++) d[i] = (int64_t)((p * A095332_R[i]) / 30);
    int64_t P = (int64_t)p;
    if (cb < 0) {
        for (int i = 0; i < 8; i++) {
            int64_t idx = cb + d[i];
            if (idx >= 0 && idx < len) seg[idx] &= mk[i];
        }
        if (cb + d[7] >= len) return cb - len;
        cb += P;
    }
    int64_t lim = len - d[7];
    unsigned char m0 = mk[0], m1 = mk[1], m2 = mk[2], m3 = mk[3];
    unsigned char m4 = mk[4], m5 = mk[5], m6 = mk[6], m7 = mk[7];
    int64_t d0 = d[0], d1 = d[1], d2 = d[2], d3 = d[3];
    int64_t d4 = d[4], d5 = d[5], d6 = d[6], d7 = d[7];
    while (cb < lim) {
        unsigned char *s = seg + cb;
        s[d0] &= m0; s[d1] &= m1; s[d2] &= m2; s[d3] &= m3;
        s[d4] &= m4; s[d5] &= m5; s[d6] &= m6; s[d7] &= m7;
        cb += P;
    }
    if (cb < len) {
        for (int i = 0; i < 8; i++) {
            int64_t idx = cb + d[i];
            if (idx < len) seg[idx] &= mk[i];
        }
    }
    return cb - len;
}

static void A095332_init(void)
{
    if (A095332_pat) return;
    A095332_pop16[0] = 0;
    for (unsigned i = 1; i < 65536u; i++)
        A095332_pop16[i] = (unsigned char)(A095332_pop16[i >> 1] + (i & 1u));
    for (int i = 0; i < 30; i++) A095332_ridx[i] = -1;
    for (int b = 0; b < 8; b++) A095332_ridx[A095332_R[b]] = (signed char)b;
    for (int b = 0; b < 8; b++) {
        unsigned c = A095332_R[b];
        for (int i = 0; i < 8; i++)
            A095332_pm[c][i] =
                (unsigned char)~(1u << A095332_ridx[(c * A095332_R[i]) % 30]);
    }
    for (int th = 0; th <= 12; th++) {
        for (unsigned e = 0; e < 2048u; e++) {
            unsigned l = 2 * e, m = 0;
            for (int k = 0; k < 8; k++)
                if ((int)A095332_pop16[l + A095332_R[k]] >= th) m |= 1u << k;
            A095332_qt[th][e] = (unsigned char)m;
        }
    }
    unsigned char *pat = (unsigned char *)malloc(A095332_PAT);
    if (!pat) abort();
    memset(pat, 0xff, A095332_PAT);
    static const unsigned sp[5] = {7, 11, 13, 17, 19};
    for (int s = 0; s < 5; s++)
        (void)A095332_cross(pat, (int64_t)A095332_PAT, sp[s], 0);
    A095332_pat = pat;
}

static uint64_t A095332_count(unsigned n)
{
    unsigned L = n + 1;
    if (L <= 6) return 0;
    A095332_init();

    uint64_t lo = (uint64_t)1 << n;
    uint64_t hi = (uint64_t)1 << (n + 1);
    unsigned need = (L + 6) / 2 + 1;           /* minimal number of 1-bits */
    if (need > L) return 0;

    uint64_t r = (uint64_t)1 << ((n + 1) / 2);
    while ((r + 1) * (r + 1) <= hi) r++;

    unsigned char *comp = (unsigned char *)calloc((size_t)r + 1, 1);
    if (!comp) abort();
    size_t np = 0, capp = 1024;
    uint64_t *pr = (uint64_t *)malloc(capp * sizeof(uint64_t));
    if (!pr) abort();
    for (uint64_t i = 2; i <= r; i++) {
        if (comp[i]) continue;
        if (i >= 23) {
            if (np == capp) {
                capp <<= 1;
                uint64_t *t = (uint64_t *)realloc(pr, capp * sizeof(uint64_t));
                if (!t) abort();
                pr = t;
            }
            pr[np++] = i;
        }
        for (uint64_t j = i * i; j <= r; j += i) comp[j] = 1;
    }
    free(comp);

    uint64_t jlo = lo / 30, jhi = (hi - 1) / 30;
    int64_t *cbs = (int64_t *)malloc((np ? np : 1) * sizeof(int64_t));
    unsigned char *seg = (unsigned char *)malloc(A095332_SEG);
    if (!cbs || !seg) abort();
    for (size_t i = 0; i < np; i++) {
        uint64_t p = pr[i];
        uint64_t c0 = jlo / p;
        cbs[i] = (int64_t)(p * c0) - (int64_t)jlo;
    }

    uint64_t cnt = 0;
    for (uint64_t j0 = jlo; j0 <= jhi; ) {
        uint64_t rem = jhi - j0 + 1;
        uint32_t len = (rem < A095332_SEG) ? (uint32_t)rem : A095332_SEG;

        uint32_t off = (uint32_t)(j0 % A095332_PAT), filled = 0;
        while (filled < len) {
            uint32_t chunk = A095332_PAT - off;
            if (chunk > len - filled) chunk = len - filled;
            memcpy(seg + filled, A095332_pat + off, chunk);
            filled += chunk;
            off = 0;
        }

        for (size_t i = 0; i < np; i++)
            cbs[i] = A095332_cross(seg, (int64_t)len, pr[i], cbs[i]);

        if (j0 == jlo) {
            for (int k = 0; k < 8; k++)
                if (jlo * 30 + A095332_R[k] <= lo) seg[0] &= (unsigned char)~(1u << k);
        }
        if (j0 + len - 1 == jhi) {
            for (int k = 0; k < 8; k++)
                if (jhi * 30 + A095332_R[k] >= hi)
                    seg[len - 1] &= (unsigned char)~(1u << k);
        }

        uint64_t bb = j0 * 30;
        uint32_t t = 0;
        while (t < len) {
            uint64_t l = bb & 4095u;
            if (l > 4066u) {
                unsigned b = seg[t];
                for (int k = 0; k < 8; k++)
                    if (((b >> k) & 1u) && A095332_pop64(bb + A095332_R[k]) >= need)
                        cnt++;
                t++;
                bb += 30;
                continue;
            }
            uint32_t run = (uint32_t)((4066u - l) / 30) + 1;
            if (run > len - t) run = len - t;
            int th = (int)need - (int)A095332_pop64(bb >> 12);
            if (th <= 0) {
                for (uint32_t u = 0; u < run; u++) cnt += A095332_pop16[seg[t + u]];
            } else if (th <= 12) {
                const unsigned char *q = A095332_qt[th];
                uint32_t e = (uint32_t)(l >> 1);
                for (uint32_t u = 0; u < run; u++, e += 15)
                    cnt += A095332_pop16[seg[t + u] & q[e]];
            }
            t += run;
            bb += (uint64_t)30 * run;
        }
        j0 += len;
    }

    free(pr); free(cbs); free(seg);
    return cnt;
}

seq_t a(idx_t n)
{
    static uint64_t cache[64];
    static bool have[64];
    if (n == 0 || n > 62) return 0;
    unsigned k = (unsigned)n;
    if (!have[k]) {
        cache[k] = A095332_count(k);
        have[k] = true;
    }
    return (seq_t)cache[k];
}
