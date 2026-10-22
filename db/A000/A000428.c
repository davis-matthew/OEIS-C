/*
 * A000428  Euler transform of A000579.
 * https://oeis.org/A000428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Euler transform of b(k) = A000579(k+5) = binomial(k+5,6): G.f.
 *            Product_{k>=1} 1/(1-x^k)^binomial(k+5,6); equivalently a(n) =
 *            (1/n) * Sum_{k=1..n} c(k)*a(n-k), c(k) = Sum_{d|k}
 *            d*binomial(d+5,6), a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 38, u128 n <= 85,
 *            big any n
 * Helpers:   is_fixed_width, rising_binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_fixed_width.h"
#include "../utils/rising_binom.h"

static seq_t *cache = NULL;
static idx_t cache_n = 0;   /* cache valid for indices 0..cache_n */

static void compute_fixed(idx_t N) {
    seq_t *poly = (seq_t *)calloc((size_t)(N + 1), sizeof(seq_t));
    seq_t *cj = (seq_t *)calloc((size_t)(N + 1), sizeof(seq_t));
    for (idx_t i = 0; i <= N; i++) poly[i] = 0;
    poly[0] = 1;
    for (idx_t k = 1; k <= N; k++) {
        seq_t m = rising_binom((seq_t)k, 6);
        idx_t jmax = N / k;
        cj[0] = 1;
        for (idx_t j = 1; j <= jmax; j++) {
            /* cj[j] = cj[j-1] * (m+j-1) / j, exactly */
            seq_t coef = cj[j - 1];
            seq_t t = m + (j - 1);
            unsigned long long x = (unsigned long long)(coef % j), y = (unsigned long long)j;
            while (y) { unsigned long long tt = x % y; x = y; y = tt; }
            coef /= x;
            t /= (j / x);
            cj[j] = coef * t;
        }
        for (idx_t i = N; i >= k; i--) {
            seq_t s = 0;
            idx_t jm = i / k;
            for (idx_t j = 1; j <= jm; j++) s += cj[j] * poly[i - k * j];
            poly[i] += s;
            if (i == k) break;
        }
    }
    free(cj);
    if (cache) free(cache);
    cache = poly;
    cache_n = N;
}

static void compute_big(idx_t N) {
    seq_t *c = (seq_t *)calloc((size_t)(N + 1), sizeof(seq_t));
    seq_t *A = (seq_t *)calloc((size_t)(N + 1), sizeof(seq_t));
    for (idx_t i = 0; i <= N; i++) { c[i] = 0; A[i] = 0; }
    for (idx_t d = 1; d <= N; d++) {
        seq_t v = rising_binom((seq_t)d, 6) * d;
        for (idx_t k = d; k <= N; k += d) c[k] += v;
    }
    idx_t start = 1;
    if (cache) {
        for (idx_t i = 0; i <= cache_n; i++) A[i] = cache[i];
        start = cache_n + 1;
        free(cache);
    } else {
        A[0] = 1;
    }
    for (idx_t n = start; n <= N; n++) {
        seq_t s = 0;
        for (idx_t k = 1; k <= n; k++) s += c[k] * A[n - k];
        A[n] = s / n;
    }
    free(c);
    cache = A;
    cache_n = N;
}

seq_t a(idx_t n) {
    if (cache == NULL || n > cache_n) {
        idx_t N = n;
        if (cache && N < 2 * cache_n) N = 2 * cache_n;
        if (N < 16) N = 16;
        if (is_fixed_width()) {
            if (cache && cache_n >= n) return cache[n];
            compute_fixed(n > 16 ? n : 16);
        } else {
            compute_big(N);
        }
    }
    return cache[n];
}
