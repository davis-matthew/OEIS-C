/*
 * A003069  Number of n-node digraphs with same converse as complement.
 * https://oeis.org/A003069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{partitions lambda of n with no part == 2 (mod 4)}
 *            2^c(lambda) / z(lambda), where z(lambda) = Prod_k k^m_k * m_k!,
 *            and c(lambda) = Sum_{parts k} s(k) + Sum_{pairs of parts i<j}
 *            k_i*k_j / lcm(k_i', k_j'), with s(k) = (k-1)/2 for k odd, s(k) =
 *            k-1 for k == 0 (mod 4), and k' = k/gcd(k,2)....
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 11, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   a3069_divs, a3069_gcd, a3069_self
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a3069_divs.h"
#include "../utils/a3069_gcd.h"
#include "../utils/a3069_self.h"

static int g_n;
static int *g_parts;
static uint32_t *g_cnt;
static uint32_t *g_S;
static size_t g_CL, g_SL;
static int *g_tab;

static uint64_t a3069_G(int a, int k)
{
    uint64_t ap, kp, g;
    if (g_tab) return (uint64_t)g_tab[(size_t)a * (size_t)(g_n + 1) + (size_t)k];
    ap = (a % 2 == 0) ? (uint64_t)a / 2 : (uint64_t)a;
    kp = (k % 2 == 0) ? (uint64_t)k / 2 : (uint64_t)k;
    g = a3069_gcd(ap, kp);
    return (uint64_t)a * (uint64_t)k / (ap / g * kp);
}

static void a3069_addshift(const uint32_t *x, uint64_t c)
{
    size_t off = (size_t)(c >> 5);
    unsigned sh = (unsigned)(c & 31);
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < g_CL; i++) {
        uint64_t v = (uint64_t)x[i] << sh;
        uint64_t t = (uint64_t)g_S[off + i] + (v & 0xffffffffULL) + carry;
        g_S[off + i] = (uint32_t)t;
        carry = (t >> 32) + (v >> 32);
    }
    i = off + g_CL;
    while (carry && i < g_SL) {
        uint64_t t = (uint64_t)g_S[i] + carry;
        g_S[i] = (uint32_t)t;
        carry = t >> 32;
        i++;
    }
}

static void a3069_rec(int rem, int maxk, int mult, uint64_t c, int d)
{
    int k;
    uint32_t *cur = g_cnt + (size_t)d * g_CL;
    uint32_t *nxt = cur + g_CL;
    if (rem == 0) {
        a3069_addshift(cur, c);
        return;
    }
    for (k = (rem < maxk ? rem : maxk); k >= 1; k--) {
        int nm, i;
        uint64_t nc;
        if (k % 4 == 2) continue;
        nm = (d > 0 && g_parts[d - 1] == k) ? mult + 1 : 1;
        nc = c + a3069_self(k);
        for (i = 0; i < d; i++) nc += a3069_G(g_parts[i], k);
        memcpy(nxt, cur, g_CL * sizeof(uint32_t));
        a3069_divs(nxt, g_CL, (uint32_t)k * (uint32_t)nm);
        g_parts[d] = k;
        a3069_rec(rem - k, k, nm, nc, d + 1);
    }
}

seq_t a(idx_t n)
{
    int N = (int)n, i, bitsn = 0;
    uint64_t factbits = 0, maxc;
    seq_t r = 0;
    size_t j;
    if (N < 1) return 0;
    g_n = N;
    while ((1ULL << bitsn) < (uint64_t)N + 1) bitsn++;
    factbits = (uint64_t)N * (uint64_t)bitsn + 64;
    g_CL = (size_t)(factbits / 32 + 2);
    maxc = (uint64_t)N * (uint64_t)(N - 1) / 2;
    g_SL = (size_t)(maxc / 32) + g_CL + 4;

    g_parts = (int *)malloc(sizeof(int) * (size_t)(N + 1));
    g_cnt = (uint32_t *)calloc((size_t)(N + 2) * g_CL, sizeof(uint32_t));
    g_S = (uint32_t *)calloc(g_SL, sizeof(uint32_t));
    g_tab = NULL;
    if (N <= 512) {
        int x, y;
        g_tab = (int *)malloc(sizeof(int) * (size_t)(N + 1) * (size_t)(N + 1));
        for (x = 1; x <= N; x++)
            for (y = 1; y <= N; y++) {
                uint64_t ap = (x % 2 == 0) ? (uint64_t)x / 2 : (uint64_t)x;
                uint64_t kp = (y % 2 == 0) ? (uint64_t)y / 2 : (uint64_t)y;
                uint64_t g = a3069_gcd(ap, kp);
                g_tab[(size_t)x * (size_t)(N + 1) + (size_t)y] =
                    (int)((uint64_t)x * (uint64_t)y / (ap / g * kp));
            }
    }

    /* cnt[0] = n! */
    g_cnt[0] = 1;
    for (i = 2; i <= N; i++) {
        uint64_t carry = 0;
        for (j = 0; j < g_CL; j++) {
            uint64_t t = (uint64_t)g_cnt[j] * (uint64_t)i + carry;
            g_cnt[j] = (uint32_t)t;
            carry = t >> 32;
        }
    }

    a3069_rec(N, N, 0, 0, 0);

    for (i = 2; i <= N; i++) a3069_divs(g_S, g_SL, (uint32_t)i);

    j = g_SL;
    while (j > 0 && g_S[j - 1] == 0) j--;
    while (j-- > 0) {
        r = (seq_t)(r << 16);
        r += (idx_t)(g_S[j] >> 16);
        r = (seq_t)(r << 16);
        r += (idx_t)(g_S[j] & 0xffffu);
    }

    free(g_parts);
    free(g_cnt);
    free(g_S);
    if (g_tab) free(g_tab);
    g_tab = NULL;
    return r;
}
