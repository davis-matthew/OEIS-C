/*
 * A001970  Functional determinants; partitions of partitions; Euler transform
 *          applied twice to all 1's sequence.
 * https://oeis.org/A001970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy, changed
 * Formula:   G.f.: Product_{k>=1} 1/(1-x^k)^p(k), p = A000041. Arbitrary
 *            precision: a(n) = (1/n)*Sum_{k=1..n} a(n-k)*b(k), a(0) = 1, b(k)
 *            = Sum_{d|k} d*p(d), p(m) from Euler's pentagonal recurrence.
 *            Fixed width (no division possible): multiply the series by
 *            1/(1-x^k)^p(k) = Sum_j C(p(k)+j-1, j) x^(k*j), binom...
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 37, u64 n <= 86, u128 n <= 197,
 *            big any n
 * Helpers:   a1970_grow, a1970_inv, a1970_is_fixed, a1970_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a1970_grow.h"
#include "../utils/a1970_inv.h"
#include "../utils/a1970_is_fixed.h"
#include "../utils/a1970_width.h"

#define A1970_FMAX 400

static seq_t a1970_fa[A1970_FMAX + 1];
static int a1970_fdone = 0;

static void a1970_fixed_build(void) {
    static uint64_t p[A1970_FMAX + 1];
    static seq_t coef[A1970_FMAX + 1];
    int N = A1970_FMAX;
    int w = a1970_width();
    for (int i = 0; i <= N; i++) p[i] = 0;
    p[0] = 1;
    for (int part = 1; part <= N; part++)
        for (int s = part; s <= N; s++) p[s] += p[s - part];
    for (int i = 0; i <= N; i++) a1970_fa[i] = 0;
    a1970_fa[0] = 1;
    for (int k = 1; k <= N; k++) {
        uint64_t P = p[k];
        int J = N / k;
        seq_t odd = 1;
        int e = 0;
        coef[0] = 1;
        for (int t = 1; t <= J; t++) {
            uint64_t num = P + (uint64_t)t - 1;
            while ((num & 1) == 0) { num >>= 1; e++; }
            odd = odd * (seq_t)num;
            uint64_t den = (uint64_t)t;
            while ((den & 1) == 0) { den >>= 1; e--; }
            odd = odd * a1970_inv((seq_t)den);
            if (e >= w) coef[t] = 0;
            else coef[t] = odd << e;
        }
        for (int m = N; m >= k; m--) {
            seq_t s = 0;
            for (int j = 1; j * k <= m; j++)
                s += coef[j] * a1970_fa[m - j * k];
            a1970_fa[m] += s;
        }
    }
    a1970_fdone = 1;
}

static seq_t *a1970_p = NULL;
static seq_t *a1970_b = NULL;
static seq_t *a1970_a = NULL;
static idx_t a1970_cap = 0;

static void a1970_big_extend(idx_t need) {
    if (need <= a1970_cap) return;
    idx_t newcap = a1970_cap * 2;
    if (newcap < need) newcap = need;
    idx_t old = a1970_cap;
    a1970_p = a1970_grow(a1970_p, old, newcap);
    a1970_b = a1970_grow(a1970_b, old, newcap);
    a1970_a = a1970_grow(a1970_a, old, newcap);
    for (idx_t m = old; m < newcap; m++) {
        if (m == 0) { a1970_p[0] = 1; continue; }
        seq_t pos = 0, neg = 0;
        for (idx_t j = 1;; j++) {
            idx_t g1 = j * (3 * j - 1) / 2;
            if (g1 > m) break;
            idx_t g2 = j * (3 * j + 1) / 2;
            if (j & 1) {
                pos += a1970_p[m - g1];
                if (g2 <= m) pos += a1970_p[m - g2];
            } else {
                neg += a1970_p[m - g1];
                if (g2 <= m) neg += a1970_p[m - g2];
            }
        }
        a1970_p[m] = pos - neg;
    }
    for (idx_t k = (old > 1 ? old : 1); k < newcap; k++) {
        seq_t s = 0;
        for (idx_t d = 1; d * d <= k; d++) {
            if (k % d == 0) {
                s += (seq_t)d * a1970_p[d];
                idx_t q = k / d;
                if (q != d) s += (seq_t)q * a1970_p[q];
            }
        }
        a1970_b[k] = s;
    }
    for (idx_t i = old; i < newcap; i++) {
        if (i == 0) { a1970_a[0] = 1; continue; }
        seq_t s = 0;
        for (idx_t k = 1; k <= i; k++) s += a1970_b[k] * a1970_a[i - k];
        a1970_a[i] = s / (seq_t)i;
    }
    a1970_cap = newcap;
}

seq_t a(idx_t n) {
    if (a1970_is_fixed()) {
        if (n > A1970_FMAX) return 0;
        if (!a1970_fdone) a1970_fixed_build();
        return a1970_fa[n];
    }
    a1970_big_extend(n + 1);
    return a1970_a[n];
}
