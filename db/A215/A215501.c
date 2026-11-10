/*
 * A215501  Inverse of permutation in A215366.
 * https://oeis.org/A215501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = A000070(A056239(n)-1) + #{ m <= n : A056239(m) =
 *            A056239(n) }, with a(1) = 1. Numbers with prime-index weight s
 *            fill the block A000070(s-1)+1..A000070(s) in increasing order
 * Range:     u8 n <= 40, u16 n <= 148, u32 n <= 598, u64 n <= 2550,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9175 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *A000070_alloc_series(size_t sz)
{
    seq_t *p = mem_new(seq_t, sz);
    size_t i;
    for (i = 0; i < sz; i++) p[i] = 0;
    return p;
}

static seq_t *A000070_P_tab = 0;
static seq_t *A000070_S_tab = 0;
static idx_t A000070_have = 0;
static idx_t A000070_cap = 0;

static void A000070_ensure(idx_t n)
{
    idx_t m, i;
    if (n + 1 > A000070_cap) {
        idx_t newcap = A000070_cap * 2;
        seq_t *NP, *NS;
        if (newcap < n + 1) newcap = n + 1;
        if (newcap < 64) newcap = 64;
        NP = A000070_alloc_series((size_t)newcap);
        NS = A000070_alloc_series((size_t)newcap);
        for (i = 0; i < A000070_have; i++) {
            NP[i] = A000070_P_tab[i];
            NS[i] = A000070_S_tab[i];
        }
        A000070_P_tab = NP;
        A000070_S_tab = NS;
        A000070_cap = newcap;
    }
    for (m = A000070_have; m <= n; m++) {
        if (m == 0) {
            A000070_P_tab[0] = 1;
            A000070_S_tab[0] = 1;
        } else {
            seq_t pos = 0, neg = 0;
            idx_t k;
            for (k = 1;; k++) {
                idx_t g1 = k * (3 * k - 1) / 2;
                idx_t g2 = k * (3 * k + 1) / 2;
                if (g1 > m) break;
                if (k & 1) {
                    pos += A000070_P_tab[m - g1];
                    if (g2 <= m) pos += A000070_P_tab[m - g2];
                } else {
                    neg += A000070_P_tab[m - g1];
                    if (g2 <= m) neg += A000070_P_tab[m - g2];
                }
            }
            A000070_P_tab[m] = pos - neg;
            A000070_S_tab[m] = A000070_S_tab[m - 1] + A000070_P_tab[m];
        }
    }
    if (n + 1 > A000070_have) A000070_have = n + 1;
}

static seq_t A000070_a(idx_t n)
{
    if (n >= A000070_have) A000070_ensure(n);
    return A000070_S_tab[n];
}

static uint32_t *wt = 0;
static idx_t wt_cap = 0;

static void build_weights(idx_t limit)
{
    idx_t L = wt_cap * 2;
    uint32_t *spf, *pidx, *w;
    uint32_t cnt = 0;
    uint64_t i, j;
    if (L < limit) L = limit;
    if (L < 1024) L = 1024;
    spf = (uint32_t *)calloc((size_t)L + 1, sizeof(uint32_t));
    pidx = (uint32_t *)calloc((size_t)L + 1, sizeof(uint32_t));
    w = (uint32_t *)calloc((size_t)L + 1, sizeof(uint32_t));
    for (i = 2; i <= (uint64_t)L; i++) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            pidx[i] = ++cnt;
            for (j = i * i; j <= (uint64_t)L; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
        w[i] = w[i / spf[i]] + pidx[spf[i]];
    }
    free(spf);
    free(pidx);
    free(wt);
    wt = w;
    wt_cap = L;
}

seq_t a(idx_t n)
{
    idx_t m, rank = 0;
    uint32_t s;
    if (n <= 1) return (seq_t)1;
    if (n > wt_cap) build_weights(n);
    s = wt[n];
    for (m = 2; m <= n; m++)
        if (wt[m] == s) rank++;
    return A000070_a((idx_t)(s - 1)) + (seq_t)rank;
}
