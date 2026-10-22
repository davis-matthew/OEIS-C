/*
 * A000613  Number of Boolean functions of n variables.
 * https://oeis.org/A000613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = A000585(n)/2 = number of orbits of GL(n,2) on subsets of
 *            the nonzero vectors of F_2^n. Burnside's lemma is summed over
 *            the conjugacy classes of GL(n,2). The classes are enumerated by
 *            rational canonical form: for each irreducible f != x, a
 *            partition is chosen. The number of cycles of a matr...
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   b613_deg, b613_gcd, b613_mod, b613_mulmod, b613_order, b613_phi,
 *            b613_powx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/b613_deg.h"
#include "../utils/b613_gcd.h"
#include "../utils/b613_mod.h"
#include "../utils/b613_mulmod.h"
#include "../utils/b613_order.h"
#include "../utils/b613_phi.h"
#include "../utils/b613_powx.h"

static uint64_t *b613_poly; static int *b613_pdeg; static uint64_t *b613_pord;
static int b613_np, b613_pcapp;
static int *b613_poff, *b613_plen, *b613_psum, *b613_pool;
static int b613_pc, b613_pcap, b613_poolsz, b613_poolcap;
static int b613_sdeg[64], b613_spart[64]; static uint64_t b613_sord[64]; static int b613_sn;
static uint32_t *b613_S, *b613_O, *b613_X; static size_t b613_SL, b613_OL; static long long b613_t;

static void b613_store(const int *buf, int len, int total)
{
    if (b613_pc == b613_pcap) {
        b613_pcap = b613_pcap ? 2 * b613_pcap : 64;
        b613_poff = (int *)realloc(b613_poff, sizeof(int) * b613_pcap);
        b613_plen = (int *)realloc(b613_plen, sizeof(int) * b613_pcap);
        b613_psum = (int *)realloc(b613_psum, sizeof(int) * b613_pcap);
    }
    if (b613_poolsz + len > b613_poolcap) {
        if (!b613_poolcap) b613_poolcap = 256;
        while (b613_poolsz + len > b613_poolcap) b613_poolcap *= 2;
        b613_pool = (int *)realloc(b613_pool, sizeof(int) * b613_poolcap);
    }
    b613_poff[b613_pc] = b613_poolsz; b613_plen[b613_pc] = len; b613_psum[b613_pc] = total;
    memcpy(b613_pool + b613_poolsz, buf, sizeof(int) * len);
    b613_poolsz += len; b613_pc++;
}

static void b613_gen(int rem, int maxp, int *buf, int len, int total)
{
    int p;
    if (rem == 0) { if (len > 0) b613_store(buf, len, total); return; }
    for (p = rem < maxp ? rem : maxp; p >= 1; p--) {
        buf[len] = p;
        b613_gen(rem - p, p, buf, len + 1, total);
    }
}

static void b613_divX(uint32_t v)
{
    uint64_t rem = 0; size_t i;
    if (v <= 1) return;
    for (i = b613_OL; i-- > 0;) {
        uint64_t cur = (rem << 32) | b613_X[i];
        b613_X[i] = (uint32_t)(cur / v); rem = cur % v;
    }
}

static void b613_addsh(long long sh)
{
    size_t w = (size_t)(sh / 32), i; int bs = (int)(sh % 32);
    uint64_t carry = 0, cur;
    for (i = 0; i <= b613_OL; i++) {
        uint64_t xi = i < b613_OL ? b613_X[i] : 0, xp = i > 0 ? b613_X[i - 1] : 0;
        uint32_t y = (uint32_t)((((xi << 32) | xp) << bs) >> 32);
        cur = (uint64_t)b613_S[w + i] + y + carry;
        b613_S[w + i] = (uint32_t)cur; carry = cur >> 32;
    }
    for (i = w + b613_OL + 1; carry && i < b613_SL; i++) {
        cur = (uint64_t)b613_S[i] + carry;
        b613_S[i] = (uint32_t)cur; carry = cur >> 32;
    }
}

static void b613_eval(void)
{
    uint64_t N = 1, g, sum = 0, c;
    int u = 0, b, i, j, k;
    long long s = 0;
    for (b = 0; b < b613_sn; b++) {
        int mp = b613_pool[b613_poff[b613_spart[b]]];
        N = N / b613_gcd(N, b613_sord[b]) * b613_sord[b];
        k = 0; while ((1 << k) < mp) k++;
        if (k > u) u = k;
    }
    N <<= u;
    for (g = 1; g <= N; g++) {
        int v = 0, D = 0, cap;
        uint64_t gg = g;
        if (N % g) continue;
        while (!(gg & 1)) { gg >>= 1; v++; }
        cap = v >= 30 ? (1 << 30) : (1 << v);
        for (b = 0; b < b613_sn; b++)
            if (g % b613_sord[b] == 0) {
                const int *p = b613_pool + b613_poff[b613_spart[b]];
                int len = b613_plen[b613_spart[b]], sm = 0;
                for (i = 0; i < len; i++) sm += p[i] < cap ? p[i] : cap;
                D += b613_sdeg[b] * sm;
            }
        sum += b613_phi(N / g) << D;
    }
    c = sum / N;
    memcpy(b613_X, b613_O, sizeof(uint32_t) * b613_OL);
    for (b = 0; b < b613_sn; b++) {
        const int *p = b613_pool + b613_poff[b613_spart[b]];
        int len = b613_plen[b613_spart[b]], d = b613_sdeg[b];
        long long sq = 0;
        for (k = 1; k <= p[0]; k++) {
            long long cnt = 0;
            for (i = 0; i < len; i++) if (p[i] >= k) cnt++;
            sq += cnt * cnt;
        }
        s += (long long)d * sq;
        i = 0;
        while (i < len) {
            long long m;
            j = i;
            while (j < len && p[j] == p[i]) j++;
            m = j - i;
            s -= (long long)d * m * (m + 1) / 2;
            for (k = 1; k <= m; k++) b613_divX((uint32_t)((1ull << (d * k)) - 1));
            i = j;
        }
    }
    b613_addsh((long long)c - 1 + b613_t - s);
}

static void b613_rec(int start, int rem)
{
    int j, q;
    if (rem == 0) { b613_eval(); return; }
    for (j = start; j < b613_np && b613_pdeg[j] <= rem; j++) {
        int d = b613_pdeg[j];
        for (q = 0; q < b613_pc && d * b613_psum[q] <= rem; q++) {
            b613_sdeg[b613_sn] = d; b613_sord[b613_sn] = b613_pord[j];
            b613_spart[b613_sn] = q; b613_sn++;
            b613_rec(j + 1, rem - d * b613_psum[q]);
            b613_sn--;
        }
    }
}

seq_t a(idx_t n)
{
    int N = (int)n, i, d, j, buf[64];
    size_t k, w; int bs;
    uint64_t f;
    seq_t r = 0;

    b613_pc = 0; b613_poolsz = 0;
    for (i = 1; i <= N; i++) b613_gen(i, i, buf, 0, i);

    b613_np = 0;
    for (d = 1; d <= N; d++)
        for (f = (1ull << d) | 1; f < (2ull << d); f += 2) {
            int irr = 1;
            for (j = 0; j < b613_np && 2 * b613_pdeg[j] <= d; j++)
                if (b613_mod(f, b613_poly[j], b613_pdeg[j]) == 0) { irr = 0; break; }
            if (!irr) continue;
            if (b613_np == b613_pcapp) {
                b613_pcapp = b613_pcapp ? 2 * b613_pcapp : 64;
                b613_poly = (uint64_t *)realloc(b613_poly, sizeof(uint64_t) * b613_pcapp);
                b613_pord = (uint64_t *)realloc(b613_pord, sizeof(uint64_t) * b613_pcapp);
                b613_pdeg = (int *)realloc(b613_pdeg, sizeof(int) * b613_pcapp);
            }
            b613_poly[b613_np] = f; b613_pdeg[b613_np] = d;
            b613_pord[b613_np] = b613_order(f, d);
            b613_np++;
        }

    b613_t = (long long)N * (N - 1) / 2;
    b613_OL = (size_t)N * (N + 1) / 64 + 2;
    b613_O = (uint32_t *)calloc(b613_OL, sizeof(uint32_t));
    b613_X = (uint32_t *)calloc(b613_OL, sizeof(uint32_t));
    b613_O[0] = 1;
    for (i = 2; i <= N; i++) {
        uint64_t carry = 0, m = (1ull << i) - 1;
        for (k = 0; k < b613_OL; k++) {
            uint64_t cur = (uint64_t)b613_O[k] * m + carry;
            b613_O[k] = (uint32_t)cur; carry = cur >> 32;
        }
    }
    b613_SL = (size_t)(((1ull << N) + (uint64_t)b613_t + (uint64_t)N * (N + 1) / 2) / 32) + 8;
    b613_S = (uint32_t *)calloc(b613_SL, sizeof(uint32_t));

    b613_sn = 0;
    b613_rec(0, N);

    for (i = 2; i <= N; i++) {
        uint64_t rem = 0, v = (1ull << i) - 1;
        for (k = b613_SL; k-- > 0;) {
            uint64_t cur = (rem << 32) | b613_S[k];
            b613_S[k] = (uint32_t)(cur / v); rem = cur % v;
        }
    }
    w = (size_t)(b613_t / 32); bs = (int)(b613_t % 32);
    for (k = b613_SL - w; k-- > 0;) {
        uint32_t limb = b613_S[k + w] >> bs;
        if (bs && k + w + 1 < b613_SL) limb |= b613_S[k + w + 1] << (32 - bs);
        r <<= 16;
        r <<= 16;
        r += (idx_t)limb;
    }

    free(b613_S); free(b613_X); free(b613_O);
    b613_S = b613_X = b613_O = NULL;
    return r;
}
