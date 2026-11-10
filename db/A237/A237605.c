/*
 * A237605  Numbers n such that A047842(n) | n.
 * https://oeis.org/A237605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   search. For each length L and each multiset of digit counts, D =
 *            A047842 of that multiset. A digit-by-digit DFS keeps only
 *            prefixes whose interval still contains a multiple of D, then
 *            lists those multiples and checks their digits. a(1) = 0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 128, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 47 terms through n = 926 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/mem.h"

static uint64_t *g_terms = 0;
static size_t g_nterms = 0, g_cap = 0;
static int g_doneL = 0;
static int g_init = 0;

static uint64_t g_p10[20];
static int g_L;
static uint64_t g_D;
static int g_cnt[10];
static int g_prof[10];

static void g_push(uint64_t v)
{
    if (g_nterms == g_cap) {
        g_cap = g_cap ? g_cap * 2 : 256;
        g_terms = (uint64_t *)realloc(g_terms, g_cap * sizeof(uint64_t));
    }
    g_terms[g_nterms++] = v;
}

static int g_cmp(const void *x, const void *y)
{
    uint64_t a = *(const uint64_t *)x, b = *(const uint64_t *)y;
    return a < b ? -1 : (a > b ? 1 : 0);
}

/* do the low r digits of t use exactly the remaining multiset g_cnt? */
static int g_chk_low(uint64_t t, int r)
{
    int c[10];
    for (int d = 0; d < 10; d++) c[d] = g_cnt[d];
    for (int i = 0; i < r; i++) {
        int d = (int)(t % 10);
        if (--c[d] < 0) return 0;
        t /= 10;
    }
    return 1;
}

static void g_dfs(int pos, uint64_t P)
{
    int r = g_L - pos;
    uint64_t mn = 0, mx = 0;
    int types = 0;
    for (int d = 0; d < 10; d++) {
        if (g_cnt[d]) types++;
        for (int c = 0; c < g_cnt[d]; c++) mn = mn * 10 + (uint64_t)d;
    }
    for (int d = 9; d >= 0; d--)
        for (int c = 0; c < g_cnt[d]; c++) mx = mx * 10 + (uint64_t)d;
    uint64_t base = P * g_p10[r];
    uint64_t lo = base + mn, hi = base + mx;   /* lo >= 1 since P >= 1 */
    uint64_t below = (lo - 1) / g_D;
    uint64_t q = hi / g_D - below;
    if (q == 0) return;
    if (r == 0 || q <= (uint64_t)types) {
        for (uint64_t m = (below + 1) * g_D; m <= hi; m += g_D) {
            if (g_chk_low(m - base, r)) g_push(m);
            if (hi - m < g_D) break;
        }
        return;
    }
    for (int d = 0; d < 10; d++) {
        if (g_cnt[d] > 0) {
            g_cnt[d]--;
            g_dfs(pos + 1, P * 10 + (uint64_t)d);
            g_cnt[d]++;
        }
    }
}

static void g_process_profile(void)
{
    /* length of the description */
    int dlen = 0;
    for (int d = 0; d < 10; d++) {
        int c = g_prof[d];
        if (c == 0) continue;
        dlen += (c < 10 ? 1 : 2) + 1;
    }
    if (dlen > g_L) return;
    uint64_t D = 0;
    for (int d = 0; d < 10; d++) {
        int c = g_prof[d];
        if (c == 0) continue;
        int dc = (c < 10) ? 1 : 2;
        D = D * g_p10[dc + 1] + (uint64_t)(c * 10 + d);
    }
    if (D > g_p10[g_L] - 1) return;
    if (g_prof[0] == g_L) return;
    /* x mod 9 is fixed by the digit multiset */
    int s = 0;
    for (int d = 0; d < 10; d++) s += d * g_prof[d];
    if (D % 3 == 0 && s % 3 != 0) return;
    if (D % 9 == 0 && s % 9 != 0) return;
    g_D = D;
    for (int f = 1; f < 10; f++) {
        if (g_prof[f] == 0) continue;
        for (int d = 0; d < 10; d++) g_cnt[d] = g_prof[d];
        g_cnt[f]--;
        g_dfs(1, (uint64_t)f);
    }
}

static void g_enum(int d, int rem)
{
    if (d == 9) {
        g_prof[9] = rem;
        g_process_profile();
        return;
    }
    for (int c = 0; c <= rem; c++) {
        g_prof[d] = c;
        g_enum(d + 1, rem - c);
    }
}

static void g_do_length(int L)
{
    g_L = L;
    size_t start = g_nterms;
    g_enum(0, L);
    qsort(g_terms + start, g_nterms - start, sizeof(uint64_t), g_cmp);
}

seq_t a(idx_t n)
{
    if (!g_init) {
        g_init = 1;
        g_p10[0] = 1;
        for (int i = 1; i < 20; i++) g_p10[i] = g_p10[i - 1] * 10;
        g_push(0);
    }
    while (g_nterms < (size_t)n && g_doneL < 19) {
        g_doneL++;
        g_do_length(g_doneL);
    }
    if (n < 1 || (size_t)n > g_nterms) return (seq_t)0;
    uint64_t v = g_terms[n - 1];
    unsigned char dg[24];
    int len = 0;
    if (v == 0) dg[len++] = 0;
    while (v > 0) { dg[len++] = (unsigned char)(v % 10); v /= 10; }
    seq_t r = 0;
    for (int i = len - 1; i >= 0; i--)
        r = r * 10 + (seq_t)dg[i];
    return r;
}
