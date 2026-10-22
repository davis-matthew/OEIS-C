/*
 * A004409  Expansion of ( Sum_{n = -infinity..infinity} x^(n^2) )^(-8).
 * https://oeis.org/A004409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^n * c(n), where Sum c(n) x^n = 1/theta_3(-x)^8 =
 *            Product_{k>=1} ((1+x^k)/(1-x^k))^8, i.e. c is the 8th
 *            convolution power of the overpartitions pbar(n) = 2*Sum_{k>=1}
 *            (-1)^(k+1) pbar(n-k^2), pbar(0) = 1. Equivalently n*c(n) =
 *            Sum_{j=1..n} s(j)*c(n-j), c(0) = 1, with s(j) = 16*Sum_{d|j,...
 * Range:     i8 n <= 1, i16 n <= 5, i32 n <= 14, i64 n <= 44, i128 n <= 143,
 *            big any n
 * Helpers:   alloc_zero, grow_zero, release, seq_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_big.h"

static seq_t *memo_c = NULL;
static idx_t memo_len = 0;

/* Arbitrary precision: Euler/Miller recurrence with all-positive terms, memoized. */
static void extend_big(idx_t n) {
    idx_t newlen, start, m, j, d;
    long long *s;
    if (n < memo_len) return;
    newlen = n + 1;
    memo_c = grow_zero(memo_c, memo_len, newlen);
    s = (long long *)calloc((size_t)newlen, sizeof(long long));
    for (d = 1; d < newlen; d++)
        for (m = d; m < newlen; m += 2 * d)
            s[m] += (long long)d;
    if (memo_len == 0) {
        memo_c[0] = 1;
        start = 1;
    } else {
        start = memo_len;
    }
    for (m = start; m < newlen; m++) {
        seq_t sum = 0;
        for (j = 1; j <= m; j++)
            sum += (seq_t)(16 * s[j]) * memo_c[m - j];
        memo_c[m] = sum / (seq_t)m;
    }
    free(s);
    memo_len = newlen;
}

/* Fixed width: convolution powers of positive series (no cancellation, no n*c(n) overflow). */
static seq_t compute_fixed(idx_t n) {
    seq_t *pb = alloc_zero(n + 1);
    seq_t *p2 = alloc_zero(n + 1);
    seq_t *p4 = alloc_zero(n + 1);
    seq_t r = 0;
    idx_t m, k, i, j;
    pb[0] = 1;
    for (m = 1; m <= n; m++) {
        seq_t sum = 0;
        for (k = 1; k * k <= m; k++) {
            if (k & 1) sum += pb[m - k * k];
            else sum -= pb[m - k * k];
        }
        pb[m] = 2 * sum;
    }
    for (i = 0; i <= n; i++) {
        seq_t t = 0;
        for (j = 0; j <= i; j++) t += pb[j] * pb[i - j];
        p2[i] = t;
    }
    for (i = 0; i <= n; i++) {
        seq_t t = 0;
        for (j = 0; j <= i; j++) t += p2[j] * p2[i - j];
        p4[i] = t;
    }
    for (j = 0; j <= n; j++) r += p4[j] * p4[n - j];
    release(pb, n + 1);
    release(p2, n + 1);
    release(p4, n + 1);
    return r;
}

seq_t a(idx_t n) {
    seq_t c;
    if (seq_is_big()) {
        extend_big(n);
        c = memo_c[n];
    } else {
        c = compute_fixed(n);
    }
    return (n & 1) ? -c : c;
}
