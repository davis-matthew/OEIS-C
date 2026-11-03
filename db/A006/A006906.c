/*
 * A006906  a(n) is the sum of products of terms in all partitions of n.
 * https://oeis.org/A006906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   G.f.: 1 / Product_{k>=1} (1-k*x^k); coefficients computed by the
 *            DP a_k(i) = a_{k-1}(i) + k*a_k(i-k) over k = 1..N, with a(0) = 1
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 41, u64 n <= 94, u128 n <= 211,
 *            big any n
 * Validated: standard, pass, 52 terms through n = 5623 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static void release(seq_t *v, idx_t len) {
    for (idx_t i = 0; i < len; i++) v[i] = 0;
    mem_free(v);
}

static seq_t *A006906_tab = NULL;
static idx_t A006906_len = 0;

static void A006906_build(idx_t len) {
    seq_t *dp = alloc_zero(len);
    idx_t k, i;
    dp[0] = 1;
    for (k = 1; k < len; k++) {
        seq_t kk = (seq_t)k;
        for (i = k; i < len; i++) {
            dp[i] += kk * dp[i - k];
        }
    }
    if (A006906_tab != NULL) {
        release(A006906_tab, A006906_len);
    }
    A006906_tab = dp;
    A006906_len = len;
}

seq_t a(idx_t n) {
    if (n >= A006906_len) {
        idx_t len = n + 1 + (n + 1) / 4 + 16;
        A006906_build(len);
    }
    return A006906_tab[n];
}
