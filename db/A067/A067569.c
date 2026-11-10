/*
 * A067569  Numbers n such that A000041(n) divides A006906(n).
 * https://oeis.org/A067569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  more, nonn
 * Formula:   a(k) = k-th n >= 0 (n = 0 is a(1)) such that A000041(n) divides
 *            A006906(n); found by direct search
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Validated: partial, pass, 5 terms through n = 5 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* ---- A000041 ---- */
static seq_t *A41_alloc_series(size_t sz)
{
    seq_t *p = mem_new(seq_t, sz);
    size_t i;
    for (i = 0; i < sz; i++) p[i] = 0;
    return p;
}

static seq_t *A41_memo = NULL;
static size_t A41_memo_cap = 0;
static size_t A41_memo_len = 0;

static seq_t A000041_a(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (need > A41_memo_cap) {
        size_t cap = A41_memo_cap ? A41_memo_cap : 64;
        while (cap < need) cap *= 2;
        seq_t *m = A41_alloc_series(cap);
        size_t i;
        for (i = 0; i < A41_memo_len; i++) m[i] = A41_memo[i];
        A41_memo = m;
        A41_memo_cap = cap;
    }
    while (A41_memo_len < need) {
        size_t m = A41_memo_len;
        seq_t pos = 0, neg = 0;
        if (m == 0) {
            pos = 1;
        } else {
            size_t k;
            for (k = 1;; k++) {
                size_t g1 = k * (3 * k - 1) / 2;
                size_t g2 = k * (3 * k + 1) / 2;
                if (g1 > m) break;
                if (k & 1) {
                    pos += A41_memo[m - g1];
                    if (g2 <= m) pos += A41_memo[m - g2];
                } else {
                    neg += A41_memo[m - g1];
                    if (g2 <= m) neg += A41_memo[m - g2];
                }
            }
        }
        A41_memo[m] = pos - neg;
        A41_memo_len++;
    }
    return A41_memo[n];
}

/* ---- A006906 ---- */
static seq_t *A006906_alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static void A006906_release(seq_t *v, idx_t len) {
    for (idx_t i = 0; i < len; i++) v[i] = 0;
    mem_free(v);
}

static seq_t *A006906_tab = NULL;
static idx_t A006906_len = 0;

static void A006906_build(idx_t len) {
    seq_t *dp = A006906_alloc_zero(len);
    idx_t k, i;
    dp[0] = 1;
    for (k = 1; k < len; k++) {
        seq_t kk = (seq_t)k;
        for (i = k; i < len; i++) {
            dp[i] += kk * dp[i - k];
        }
    }
    if (A006906_tab != NULL) {
        A006906_release(A006906_tab, A006906_len);
    }
    A006906_tab = dp;
    A006906_len = len;
}

static seq_t A006906_a(idx_t n) {
    if (n >= A006906_len) {
        idx_t len = n + 1 + (n + 1) / 4 + 16;
        A006906_build(len);
    }
    return A006906_tab[n];
}

/* ---- search ---- */
static idx_t *A067569_terms = NULL;
static size_t A067569_cnt = 0;
static size_t A067569_cap = 0;
static idx_t A067569_next = 0;

seq_t a(idx_t n)
{
    while (A067569_cnt < (size_t)n) {
        idx_t m = A067569_next++;
        seq_t p = A000041_a(m);
        if (p == 0) continue;
        seq_t s = A006906_a(m);
        if (s % p == 0) {
            if (A067569_cnt == A067569_cap) {
                size_t nc = A067569_cap ? A067569_cap * 2 : 64;
                idx_t *t = (idx_t *)malloc(nc * sizeof(idx_t));
                size_t i;
                for (i = 0; i < A067569_cnt; i++) t[i] = A067569_terms[i];
                if (A067569_terms) free(A067569_terms);
                A067569_terms = t;
                A067569_cap = nc;
            }
            A067569_terms[A067569_cnt++] = m;
        }
    }
    return (seq_t)A067569_terms[n - 1];
}
