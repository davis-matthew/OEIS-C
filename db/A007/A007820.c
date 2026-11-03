/*
 * A007820  Stirling numbers of second kind S(2n,n).
 * https://oeis.org/A007820
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1/n! * Sum_{k = 0..n} binomial(n,k)*(-1)^k*(n-k)^(2*n)
 *            (fixed-width builds instead use S(i,j) = j*S(i-1,j) +
 *            S(i-1,j-1), S(0,0) = 1, which is exact modulo 2^w since it only
 *            adds and multiplies)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 301 (2026-11-03)
 * Helpers:   ipow_idx, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"
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

static bool seq_is_fixed_width(void)
{
    seq_t t = 1;
    int i;
    for (i = 0; i < 256; i++) {
        t *= 2;
        if (t == 0)
            return true;
    }
    return false;
}

static seq_t a007820_dp(idx_t n) {
    idx_t N = 2 * n;
    idx_t K = n;
    seq_t *prev = alloc_zero(K + 1);
    seq_t *curr = alloc_zero(K + 1);
    seq_t *temp;
    seq_t result;
    idx_t i, j;

    prev[0] = 1;
    for (i = 1; i <= N; i++) {
        idx_t max_j = (i < K) ? i : K;
        curr[0] = 0;
        for (j = 1; j <= max_j; j++)
            curr[j] = (seq_t)j * prev[j] + prev[j - 1];
        temp = prev;
        prev = curr;
        curr = temp;
    }
    result = prev[K];
    release(curr, K + 1);
    release(prev, K + 1);
    return result;
}

static seq_t a007820_formula(idx_t n) {
    seq_t pos = 0, neg = 0, c = 1, fact = 1;
    idx_t k;

    for (k = 0; k <= n; k++) {
        seq_t term = c * ipow_idx((seq_t)(n - k), 2 * n);
        if (k % 2 == 0)
            pos += term;
        else
            neg += term;
        if (k < n)
            c = c * (seq_t)(n - k) / (seq_t)(k + 1);
    }
    for (k = 2; k <= n; k++)
        fact *= (seq_t)k;
    return (pos - neg) / fact;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (seq_is_fixed_width())
        return a007820_dp(n);
    return a007820_formula(n);
}
