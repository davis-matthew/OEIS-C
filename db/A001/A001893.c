/*
 * A001893  Number of permutations of (1,...,n) having n-3 inversions (n>=3).
 * https://oeis.org/A001893
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [q^(n-3)] Product_{i=1..n} (1-q^i)/(1-q); since n-3 < n,
 *            by Euler's pentagonal theorem a(n) = Sum_{j} (-1)^j *
 *            binomial(2n-4-g_j, n-1), where g_j = j(3j-1)/2 ranges over the
 *            generalized pentagonal numbers <= n-3. For fixed-width seq_t the
 *            same coefficient is computed by an additive DP (row...
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 20, u64 n <= 36, u128 n <= 68,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 978 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t a001893_big(idx_t n) {
    idx_t k = n - 3, t, j;
    signed char *sg = (signed char *)calloc(k + 1, 1);
    seq_t c = 1, pos = 0, neg = 0;
    if (!sg) return 0;
    sg[k] = 1;
    for (j = 1;; j++) {
        idx_t g1 = j * (3 * j - 1) / 2, g2 = g1 + j;
        signed char s = (j & 1) ? -1 : 1;
        if (g1 > k) break;
        sg[k - g1] = s;
        if (g2 <= k) sg[k - g2] = s;
    }
    for (t = 0; t <= k; t++) {
        if (t > 0) {
            c = c * (seq_t)(n - 1 + t);
            c = c / (seq_t)t;
        }
        if (sg[t] > 0) pos += c;
        else if (sg[t] < 0) neg += c;
    }
    free(sg);
    return pos - neg;
}

static int a001893_bigmode(void) {
    seq_t x = 1;
    int i;
    for (i = 0; i < 130; i++) x += x;
    return x != 0;
}

static seq_t a001893_fixed(idx_t n) {
    idx_t k = n - 3, i, j;
    seq_t *prev = mem_new(seq_t, k + 1);
    seq_t *curr = mem_new(seq_t, k + 1);
    seq_t *prefix = mem_new(seq_t, k + 2);
    seq_t result;
    if (!prev || !curr || !prefix) {
        mem_free(prev); mem_free(curr); mem_free(prefix);
        return 0;
    }
    prev[0] = 1;
    for (i = 1; i < n; i++) {
        seq_t *tmp;
        prefix[0] = 0;
        for (j = 0; j <= k; j++) prefix[j + 1] = prefix[j] + prev[j];
        for (j = 0; j <= k; j++) {
            idx_t lo = (j >= i) ? (j - i) : 0;
            curr[j] = prefix[j + 1] - prefix[lo];
        }
        tmp = prev; prev = curr; curr = tmp;
    }
    result = prev[k];
    mem_free(prev); mem_free(curr); mem_free(prefix);
    return result;
}

seq_t a(idx_t n) {
    if (n < 3) return 0;
    if (a001893_bigmode()) return a001893_big(n);
    return a001893_fixed(n);
}
