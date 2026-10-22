/*
 * A000686  Number of 4-colored labeled graphs on n nodes, divided by 4.
 * https://oeis.org/A000686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   Z/4Z acts freely on 4-colorings (with n >= 1 vertices) by
 *            rotating colors, so a(n) = number of 4-colored labeled graphs
 *            with vertex 1 colored 0: a(n) = Sum_{k=1..n}
 *            binomial(n-1,k-1)*2^(k*(n-k))*T3(n-k), where T3(m) =
 *            Sum_{j=0..m} binomial(m,j)*2^(j*(m-j))*T2(m-j) (3-colored
 *            labeled graphs, A0478...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   grow_arr, pw2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/grow_arr.h"
#include "../utils/pw2.h"

static seq_t *T2 = NULL, *T3 = NULL;
static idx_t cap = 0, have = 0;

static void ensure(idx_t m) {
    if (m + 1 > cap) {
        idx_t nc = cap * 2;
        if (nc < m + 1) nc = m + 1;
        T2 = grow_arr(T2, cap, nc);
        T3 = grow_arr(T3, cap, nc);
        cap = nc;
    }
    for (idx_t t = have; t <= m; t++) {
        seq_t s2 = 0;
        seq_t c = 1;
        for (idx_t j = 0; j <= t; j++) {
            s2 += c * pw2(j * (t - j));
            if (j < t) c = c * (seq_t)(t - j) / (seq_t)(j + 1);
        }
        T2[t] = s2;
        seq_t s3 = 0;
        c = 1;
        for (idx_t j = 0; j <= t; j++) {
            s3 += c * pw2(j * (t - j)) * T2[t - j];
            if (j < t) c = c * (seq_t)(t - j) / (seq_t)(j + 1);
        }
        T3[t] = s3;
    }
    if (m + 1 > have) have = m + 1;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    ensure(n - 1);
    seq_t sum = 0;
    seq_t c = 1; /* binomial(n-1, k-1) */
    for (idx_t k = 1; k <= n; k++) {
        sum += c * pw2(k * (n - k)) * T3[n - k];
        if (k < n) c = c * (seq_t)(n - k) / (seq_t)k;
    }
    return sum;
}
