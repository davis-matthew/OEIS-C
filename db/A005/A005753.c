/*
 * A005753  Number of rooted identity matched trees with n nodes.
 * https://oeis.org/A005753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, eigen
 * Formula:   G.f. A(x) = x*Product_{k>=1} (1 + x^k)^(2*a(k)) =
 *            x*Product_{k>=1} (1 - x^k)^(-c(k)), c(k) = 2*a(k) - 2*a(k/2)
 *            [second term only for even k]; with B(x) = A(x)/x, B_0 = 1:
 *            m*B_m = Sum_{i=1..m} e(i)*B_{m-i}, e(i) = Sum_{k|i} k*c(k); a(n)
 *            = B_{n-1}, a(1) = 1. Fixed-width builds instead multiply out...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 30, u128 n <= 58,
 *            big any n
 * Helpers:   alloc_zero, grow_zero, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/seq_is_fixed_width.h"

static seq_t g_gcd(seq_t x, seq_t y) {
    while (y != 0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

static seq_t a_fixed(idx_t n) {
    idx_t N = n, k, i, j;
    seq_t *B, *bin, r;
    if (N > 200) return 0; /* far beyond any fixed-width range */
    B = alloc_zero(N);
    bin = alloc_zero(N + 1);
    B[0] = 1;
    for (k = 1; k < N; k++) {
        seq_t m = B[k - 1] * 2; /* 2*a(k), final */
        idx_t J = (N - 1) / k;
        bin[0] = 1;
        for (j = 1; j <= J; j++) {
            seq_t b = bin[j - 1];
            seq_t jj = (seq_t)j;
            seq_t g = g_gcd(b, jj);
            if (g == 0) g = 1;
            bin[j] = (b / g) * ((m - jj + 1) / (jj / g));
        }
        for (i = N - 1; i >= k; i--) {
            seq_t s = 0;
            for (j = 0; j * k <= i; j++) s += bin[j] * B[i - j * k];
            B[i] = s;
        }
    }
    r = B[N - 1];
    free(B);
    free(bin);
    return r;
}

static seq_t *Bv = NULL, *Ev = NULL;
static idx_t have = 0, cap = 0;

static seq_t cval(idx_t k) {
    seq_t c = Bv[k - 1] * 2;
    if ((k & 1) == 0) c -= Bv[k / 2 - 1] * 2;
    return c;
}

seq_t a(idx_t n) {
    idx_t need, m, i, d;
    if (n == 0) return 0;
    if (seq_is_fixed_width()) return a_fixed(n);
    need = n; /* need B_0..B_{n-1} */
    if (need > cap) {
        idx_t nc = cap ? cap : 16;
        while (nc < need) nc *= 2;
        if (Bv == NULL) {
            Bv = alloc_zero(nc);
            Ev = alloc_zero(nc);
        } else {
            Bv = grow_zero(Bv, cap, nc);
            Ev = grow_zero(Ev, cap, nc);
        }
        cap = nc;
    }
    for (m = have; m < need; m++) {
        seq_t s = 0;
        if (m == 0) {
            Bv[0] = 1;
            continue;
        }
        /* e(m) = Sum_{k|m} k*c(k) */
        Ev[m] = 0;
        for (d = 1; d * d <= m; d++) {
            if (m % d == 0) {
                idx_t d2 = m / d;
                Ev[m] += cval(d) * d;
                if (d2 != d) Ev[m] += cval(d2) * d2;
            }
        }
        for (i = 1; i <= m; i++) s += Ev[i] * Bv[m - i];
        Bv[m] = s / m;
    }
    if (need > have) have = need;
    return Bv[n - 1];
}
