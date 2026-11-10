/*
 * A099521  a(n) is such that A099520(a(n))=n.
 * https://oeis.org/A099521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = position of n in A099520, where A099520(1)=1 and for k>1,
 *            A099520(k) = smallest m>0 not yet used with m + A099520(k-1)
 *            divisible by k-1; computed by generating A099520 until n appears
 * Range:     u8 n <= 30, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static seq_t *g_s = NULL;
static size_t g_len = 0, g_cap = 0;
static bool *g_used = NULL;
static size_t g_ucap = 0;

static void g_ensure_used(size_t idx)
{
    if (idx < g_ucap) return;
    size_t cap = g_ucap ? g_ucap : 64;
    while (cap <= idx) cap <<= 1;
    bool *t = (bool *)realloc(g_used, cap * sizeof(bool));
    if (!t) abort();
    memset(t + g_ucap, 0, (cap - g_ucap) * sizeof(bool));
    g_used = t;
    g_ucap = cap;
}

static void g_push(seq_t v)
{
    if (g_len + 1 >= g_cap) {
        size_t cap = g_cap ? g_cap << 1 : 64;
        seq_t *t = mem_resize(seq_t, g_s, cap);
        if (!t) abort();
        g_s = t;
        g_cap = cap;
    }
    g_s[++g_len] = v;
    size_t u = (size_t)(long long)v;
    g_ensure_used(u);
    g_used[u] = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (g_len == 0) g_push((seq_t)1);
    seq_t target = (seq_t)n;
    for (size_t i = 1; i <= g_len; ++i)
        if (g_s[i] == target) return (seq_t)i;
    for (;;) {
        size_t i = g_len + 1;
        seq_t mod = (seq_t)(i - 1);
        seq_t prev = g_s[i - 1];
        seq_t rem = prev % mod;
        seq_t cand = (rem == 0) ? mod : (mod - rem);
        for (;;) {
            g_ensure_used((size_t)(long long)cand);
            if (!g_used[(size_t)(long long)cand]) break;
            cand += mod;
        }
        g_push(cand);
        if (cand == target) return (seq_t)i;
    }
}
