/*
 * A235864  G-Lehmer numbers: Composite numbers k such that A060968(k) divides
 *          A201629(k).
 * https://oeis.org/A235864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. a(n) is the n-th composite k with A060968(k) |
 *            A201629(k). Such k are odd, squarefree and have at least two
 *            prime factors. Write k = P*q with q the largest prime and f =
 *            A060968(P). Then c*f*(q+u_q) = P*q + u_k for an integer c, where
 *            u_x = -1 if x == 1 (mod 4) and +1 if x == 3 (mod 4). T...
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 466, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 51 terms through n = 943 (2026-11-07)
 * Helpers:   gcd_u64, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/gcd_u64.h"

static uint64_t *g_terms = 0;
static size_t g_cnt = 0, g_cap = 0;
static uint64_t g_N = 0;
static uint32_t *g_pr = 0;
static size_t g_npr = 0;

static void gl_add(uint64_t k)
{
    if (g_cnt == g_cap) {
        g_cap = g_cap ? g_cap * 2 : 256;
        g_terms = (uint64_t *)realloc(g_terms, g_cap * sizeof(uint64_t));
    }
    g_terms[g_cnt++] = k;
}

static void gl_solve_final(uint64_t P, uint64_t f, uint64_t p0)
{
    uint64_t cmax = (5 * P) / (4 * f) + 2;
    uint64_t qmax = g_N / P;
    for (uint64_t c = 1; c <= cmax; ++c) {
        for (int uq = -1; uq <= 1; uq += 2) {
            for (int uk = -1; uk <= 1; uk += 2) {
                __int128 D = (__int128)P - (__int128)c * (__int128)f;
                if (D == 0) continue;
                __int128 num = (__int128)c * (__int128)f * uq - uk;
                if (num % D != 0) continue;
                __int128 qq = num / D;
                if (qq <= (__int128)p0 || qq > (__int128)qmax) continue;
                uint64_t q = (uint64_t)qq;
                if (uq == -1 ? (q & 3) != 1 : (q & 3) != 3) continue;
                uint64_t k = P * q;
                if (uk == -1 ? (k & 3) != 1 : (k & 3) != 3) continue;
                if (!is_prime_u64(q)) continue;
                gl_add(k);
            }
        }
    }
}

static void gl_dfs(uint64_t P, uint64_t f, size_t idx0)
{
    gl_solve_final(P, f, g_pr[idx0]);
    uint64_t pm = P % f;
    uint64_t r = pm < f - pm ? pm : f - pm;
    for (size_t idx = idx0 + 1; idx < g_npr; ++idx) {
        uint64_t p = g_pr[idx];
        if ((unsigned __int128)P * p * (p + 2) > (unsigned __int128)g_N) break;
        uint64_t t = g_N / P, s = 0;
        while (t >= p) { t /= p; ++s; }
        if (s <= p - 1 &&
            (unsigned __int128)(16 * r - 1) * (p - 1) >
            (unsigned __int128)32 * s * P) break;
        uint64_t up = ((p & 3) == 1) ? p - 1 : p + 1;
        if (f % p == 0) continue;
        if (gcd_u64(P, up) != 1) continue;
        gl_dfs(P * p, f * up, idx);
    }
}

static int gl_cmp(const void *x, const void *y)
{
    uint64_t a1 = *(const uint64_t *)x, b1 = *(const uint64_t *)y;
    return a1 < b1 ? -1 : (a1 > b1 ? 1 : 0);
}

static void gl_run(uint64_t N)
{
    g_N = N;
    g_cnt = 0;
    uint64_t lim = 2;
    while (lim * lim <= N) ++lim;
    lim += 2;
    uint8_t *comp = (uint8_t *)calloc((size_t)lim + 1, 1);
    free(g_pr);
    g_pr = (uint32_t *)malloc(((size_t)lim + 1) * sizeof(uint32_t));
    g_npr = 0;
    for (uint64_t i = 2; i <= lim; ++i) {
        if (comp[i]) continue;
        g_pr[g_npr++] = (uint32_t)i;
        for (uint64_t j = i * i; j <= lim; j += i) comp[j] = 1;
    }
    free(comp);
    for (size_t idx = 1; idx < g_npr; ++idx) { /* skip 2 */
        uint64_t p = g_pr[idx];
        if ((unsigned __int128)p * (p + 2) > (unsigned __int128)N) break;
        uint64_t up = ((p & 3) == 1) ? p - 1 : p + 1;
        gl_dfs(p, up, idx);
    }
    qsort(g_terms, g_cnt, sizeof(uint64_t), gl_cmp);
    size_t w = 0;
    for (size_t i = 0; i < g_cnt; ++i)
        if (w == 0 || g_terms[w - 1] != g_terms[i]) g_terms[w++] = g_terms[i];
    g_cnt = w;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (g_N == 0) gl_run((uint64_t)1 << 20);
    while (g_cnt < (size_t)n) gl_run(g_N * 4);
    return (seq_t)g_terms[(size_t)n - 1];
}
