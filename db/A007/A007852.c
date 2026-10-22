/*
 * A007852  Number of antichains in rooted plane trees on n nodes.
 * https://oeis.org/A007852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(n) = Sum_{j=1..n-1} (a(j)+b(j))*a(n-j), b(n) =
 *            C(2n-2,n-1)/n (Catalan), used for fixed-width types; arbitrary
 *            precision uses the P-recurrence (4n^3+42n^2+146n+168)*a(n+4) =
 *            (2000n^3-500n)*a(n) + (120-220n-1380n^2-920n^3)*a(n+1) +
 *            (21n^3-387n^2-1626n-1488)*a(n+2) + (37n^3+351n^2+1088n+...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   alloc_zero, release, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/seq_is_fixed_width.h"
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static void add_term(seq_t *pos, seq_t *neg, long long c, seq_t x) {
    if (c >= 0) *pos += (seq_t)(unsigned long long)c * x;
    else *neg += (seq_t)(unsigned long long)(-c) * x;
}

static seq_t a_fixed(idx_t n) {
    seq_t *cat = alloc_zero(n + 1);
    seq_t *av = alloc_zero(n + 1);
    idx_t k, i, m, j;
    seq_t r;
    cat[0] = 1;
    for (k = 1; k <= n; k++) {
        seq_t s = 0;
        for (i = 0; i < k; i++) s += cat[i] * cat[k - 1 - i];
        cat[k] = s;
    }
    av[1] = 1;
    for (m = 2; m <= n; m++) {
        seq_t s = 0;
        for (j = 1; j < m; j++) s += (av[j] + cat[j - 1]) * av[m - j];
        av[m] = s;
    }
    r = av[n];
    release(cat, n + 1);
    release(av, n + 1);
    return r;
}

static seq_t a_big(idx_t n) {
    seq_t x0 = 0, x1 = 1, x2 = 2, x3 = 7;
    idx_t m;
    if (n == 1) return x1;
    if (n == 2) return x2;
    if (n == 3) return x3;
    for (m = 0; m + 4 <= n; m++) {
        long long t = (long long)m;
        long long c0 = 2000 * t * t * t - 500 * t;
        long long c1 = 120 - 220 * t - 1380 * t * t - 920 * t * t * t;
        long long c2 = 21 * t * t * t - 387 * t * t - 1626 * t - 1488;
        long long c3 = 37 * t * t * t + 351 * t * t + 1088 * t + 1104;
        long long c4 = 4 * t * t * t + 42 * t * t + 146 * t + 168;
        seq_t pos = 0, neg = 0, nx;
        add_term(&pos, &neg, c0, x0);
        add_term(&pos, &neg, c1, x1);
        add_term(&pos, &neg, c2, x2);
        add_term(&pos, &neg, c3, x3);
        nx = (pos - neg) / (seq_t)(unsigned long long)c4;
        x0 = x1; x1 = x2; x2 = x3; x3 = nx;
    }
    return x3;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (seq_is_fixed_width()) return a_fixed(n);
    return a_big(n);
}
