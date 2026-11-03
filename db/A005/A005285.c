/*
 * A005285  Number of permutations of (1,...,n) having n-7 inversions (n>=7).
 * https://oeis.org/A005285
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j} (-1)^j * binomial(2n-8-g_j, n-1), over
 *            generalized pentagonal numbers g_j = j(3j-1)/2 <= n-7 (j in Z);
 *            i.e. a(n) = [x^(n-7)] Prod_{i=1..n} (1-x^i)/(1-x), which is the
 *            Euler pentagonal series times (1-x)^(-n) because n-7 < n
 * Range:     u8 n <= 10, u16 n <= 14, u32 n <= 22, u64 n <= 38, u128 n <= 70,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 966 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
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

static int seq_is_big(void)
{
    seq_t x = 1;
    int i;
    for (i = 0; i < 130; i++) x = x * 2;
    return !(x == 0);
}

static seq_t big_path(idx_t n) {
    idx_t k = n - 7, N = n - 1, i, j, m;
    signed char *sg = (signed char *)calloc(k + 1, 1);
    seq_t c = 1, pos = 0, neg = 0;
    sg[0] = 1;
    for (j = 1;; j++) {
        idx_t g1 = j * (3 * j - 1) / 2, g2 = j * (3 * j + 1) / 2;
        signed char s = (j & 1) ? -1 : 1;
        if (g1 > k) break;
        sg[g1] = s;
        if (g2 <= k) sg[g2] = s;
    }
    for (i = 1; i <= k; i++) {
        c *= (seq_t)(N + i);
        c /= (seq_t)i;
    }
    for (m = k + 1; m-- > 0;) {
        idx_t g = k - m;
        if (sg[g] > 0) pos += c;
        else if (sg[g] < 0) neg += c;
        if (m > 0) {
            c *= (seq_t)m;
            c /= (seq_t)(N + m);
        }
    }
    free(sg);
    return pos - neg;
}

static seq_t fixed_path(idx_t n) {
    idx_t k = n - 7, i, j;
    seq_t *dp = alloc_zero(k + 1);
    seq_t r;
    dp[0] = 1;
    for (i = 2; i <= n; i++) {
        for (j = 1; j <= k; j++) dp[j] += dp[j - 1];
        if (i <= k)
            for (j = k; j >= i; j--) dp[j] -= dp[j - i];
    }
    r = dp[k];
    release(dp, k + 1);
    return r;
}

seq_t a(idx_t n) {
    if (n < 7) return 0;
    if (seq_is_big()) return big_path(n);
    return fixed_path(n);
}
