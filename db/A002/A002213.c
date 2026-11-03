/*
 * A002213  Number of tree-like polyhexes rooted at a hexagon and containing n
 *          hexagons.
 * https://oeis.org/A002213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f.: x + x*U(x) + (3/2)*x*U(x)^2 + (1/2)*x*U(x^2) +
 *            (1/3)*x*U(x)^3 + (2/3)*x*U(x^3), U(x) = (1 - 3*x -
 *            sqrt((1-x)*(1-5*x)))/(2*x) = Sum_{k>=1} A002212(k) x^k. With m =
 *            n-1: a(n) = [m=0] + u(m) + (3*c2(m) + u(m/2))/2 + (c3(m) +
 *            2*u(m/3))/3, where c2(m) = u(m+1) - 3*u(m) - [m=0], c3(m) =
 *            c2(m+1) -...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 31, u128 n <= 59,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 200 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t a2213_big(idx_t m) {
    idx_t top = m + 2, k;
    seq_t p2 = 1, p1 = 1;          /* w(k-2), w(k-1) */
    seq_t um = 0, um1 = 0, um2 = 0, uh = 0, ut = 0;
    for (k = 1; k <= top; k++) {
        seq_t w;
        if (k == 1) {
            w = 1;
        } else {
            w = ((seq_t)(6 * k - 3) * p1 - (seq_t)(5 * (k - 2)) * p2) / (seq_t)(k + 1);
            p2 = p1;
            p1 = w;
        }
        if (k == m) um = w;
        if (k == m + 1) um1 = w;
        if (k == m + 2) um2 = w;
        if (m % 2 == 0 && k == m / 2) uh = w;
        if (m % 3 == 0 && k == m / 3) ut = w;
    }
    seq_t c2m = um1 - 3 * um;
    if (m == 0) c2m -= 1;
    seq_t c2m1 = um2 - 3 * um1;
    seq_t c3 = c2m1 - 3 * c2m - um;
    seq_t r = um + (3 * c2m + uh) / 2 + (c3 + 2 * ut) / 3;
    if (m == 0) r += 1;
    return r;
}

static seq_t a2213_fixed(idx_t m) {
    idx_t L = m + 1, k, i;
    seq_t *u = mem_new(seq_t, (size_t)(L + 1));
    for (k = 0; k <= L; k++) u[k] = 0;
    if (L >= 1) u[1] = 1;
    for (k = 2; k <= L; k++) {
        seq_t s = 3 * u[k - 1];
        for (i = 1; i + 1 < k; i++) s += u[i] * u[k - 1 - i];
        u[k] = s;
    }
    seq_t r = (m == 0) ? (seq_t)1 : (seq_t)0;
    r += u[m];
    /* U^2 coefficient and the (U^2 + U(x^2))/2 part */
    seq_t P = 0;
    for (i = 1; i < m - i && i < m; i++) P += u[i] * u[m - i];
    seq_t c2 = 2 * P, half = P;
    if (m % 2 == 0 && m > 0) {
        seq_t h = u[m / 2];
        c2 += h * h;
        if (h % 2 == 0) half += (h / 2) * (h + 1);
        else half += h * ((h + 1) / 2);
    }
    r += c2 + half;
    /* (U^3 + 2 U(x^3))/3: cyclic classes of compositions into 3 parts */
    seq_t T = 0;
    for (i = 1; 3 * i < m; i++) {
        idx_t rest = m - i, x;
        seq_t s = 0;
        for (x = i + 1; x + i + 1 <= rest; x++) s += u[x] * u[rest - x];
        T += u[i] * s;
        if (m - 2 * i > i) T += u[i] * u[i] * u[m - 2 * i];
    }
    if (m % 3 == 0 && m > 0) {
        seq_t t = u[m / 3];
        if (t % 3 == 0) T += (t / 3) * (t * t + 2);
        else T += t * ((t * t + 2) / 3);
    }
    r += T;
    mem_free(u);
    return r;
}

static int a2213_is_fixed(void) {
    seq_t x = 1;
    int i;
    for (i = 0; i < 200; i++) x = x + x;
    return x == 0;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    idx_t m = n - 1;
    if (a2213_is_fixed()) return a2213_fixed(m);
    return a2213_big(m);
}
