/*
 * A076142  a(n) = A064097(n) - A003313(n).
 * https://oeis.org/A076142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A064097(n) - A003313(n); A064097 by smallest-prime-factor
 *            sieve (a(1)=0, a(p)=1+a(p-1), a(nm)=a(n)+a(m)); A003313 by
 *            iterative-deepening addition-chain search
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 145 terms through n = 9969 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *A064097_arr = NULL;
static idx_t A064097_cap = 0;

static void A064097_build(idx_t n)
{
    idx_t sz, i;
    idx_t *spf;
    if (n < A064097_cap) return;
    sz = n + 1;
    if (sz < 1024) sz = 1024;
    if (sz < 2 * A064097_cap) sz = 2 * A064097_cap;
    if (A064097_arr) mem_free(A064097_arr);
    A064097_arr = mem_new(seq_t, sz);
    spf = (idx_t *)calloc((size_t)sz, sizeof(idx_t));
    for (i = 2; i < sz; i++) {
        if (spf[i] == 0) {
            idx_t j;
            spf[i] = i;
            if (i <= sz / i)
                for (j = i * i; j < sz; j += i)
                    if (spf[j] == 0) spf[j] = i;
        }
    }
    A064097_arr[0] = 0;
    A064097_arr[1] = 0;
    for (i = 2; i < sz; i++) {
        if (spf[i] == i) A064097_arr[i] = A064097_arr[i - 1] + 1;
        else A064097_arr[i] = A064097_arr[spf[i]] + A064097_arr[i / spf[i]];
    }
    free(spf);
    A064097_cap = sz;
}

static seq_t A064097_a(idx_t n)
{
    if (n == 0) return 0;
    A064097_build(n);
    return A064097_arr[n];
}

#define A3313_MAXD 72

static uint64_t a3313_ch[A3313_MAXD + 2];
static uint64_t a3313_tried[A3313_MAXD + 1][2700];
static int a3313_L;
static uint64_t a3313_N;

static int a3313_dfs(int k)
{
    uint64_t last = a3313_ch[k];
    int rem, i, j, nt, t, dup;
    if (last == a3313_N) return 1;
    if (k >= a3313_L) return 0;
    rem = a3313_L - k;
    if (rem < 63 && (last << rem) < a3313_N) return 0;
    nt = 0;
    for (i = k; i >= 0; i--) {
        if (2 * a3313_ch[i] <= last) break;
        for (j = i; j >= 0; j--) {
            uint64_t s = a3313_ch[i] + a3313_ch[j];
            if (s <= last) break;
            if (s > a3313_N) continue;
            if (rem - 1 < 63 && (s << (rem - 1)) < a3313_N) break;
            dup = 0;
            for (t = 0; t < nt; t++)
                if (a3313_tried[k][t] == s) { dup = 1; break; }
            if (dup) continue;
            if (nt < 2700) a3313_tried[k][nt++] = s;
            a3313_ch[k + 1] = s;
            if (a3313_dfs(k + 1)) return 1;
        }
    }
    return 0;
}

static seq_t A003313_a(idx_t n)
{
    uint64_t x;
    int lam = 0, v = 0, L;
    if (n <= 1) return 0;
    x = (uint64_t)n;
    while (x > 1) { x >>= 1; lam++; }
    x = (uint64_t)n;
    while (x) { v += (int)(x & 1); x >>= 1; }
    L = lam + (v >= 2 ? 1 : 0) + (v >= 3 ? 1 : 0);
    a3313_N = (uint64_t)n;
    a3313_ch[0] = 1;
    for (;; L++) {
        if (L > A3313_MAXD) break;
        a3313_L = L;
        if (a3313_dfs(0)) return (seq_t)L;
    }
    return (seq_t)L;
}

seq_t a(idx_t n)
{
    return A064097_a(n) - A003313_a(n);
}
