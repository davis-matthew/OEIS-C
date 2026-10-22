/*
 * A002435  Second-order Euler numbers.
 * https://oeis.org/A002435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! [x^n] sec(x)*tan(x)^2 for even n, n! [x^n]
 *            sec(x)*tan(x)^3 for odd n (this is A104035(n,2) / A104035(n,3),
 *            the coefficients of the sec-derivative polynomials Q_0 = 1,
 *            Q_{m+1}(y) = (1+y^2) Q_m'(y) + y Q_m(y)). Equivalently, with
 *            E(m) = A000111(m) (zigzag/secant numbers): a(n) = (E(n+2) -...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

#define A2435_MAXN 10016

static seq_t a2435_r0[A2435_MAXN + 8];
static seq_t a2435_r1[A2435_MAXN + 8];
static seq_t a2435_z[A2435_MAXN + 8];
static seq_t *a2435_prev = a2435_r0;
static seq_t *a2435_cur = a2435_r1;
static idx_t a2435_rows = 0; /* rows 0..a2435_rows-1 computed */

static seq_t a2435_t0[A2435_MAXN + 8];
static seq_t a2435_t1[A2435_MAXN + 8];

static int a2435_mode = -1; /* 1 = fixed width, 0 = big */

static int a2435_is_fixed(void) {
    if (a2435_mode < 0) {
        seq_t x = 1;
        int i;
        for (i = 0; i < 300; i++) x += x;
        a2435_mode = (x == 0) ? 1 : 0;
    }
    return a2435_mode;
}

static void a2435_extend(idx_t upto) {
    while (a2435_rows <= upto) {
        idx_t m = a2435_rows;
        idx_t k;
        seq_t *tmp;
        if (m == 0) {
            a2435_cur[0] = 1;
        } else {
            a2435_cur[0] = 0;
            for (k = 1; k <= m; k++)
                a2435_cur[k] = a2435_cur[k - 1] + a2435_prev[m - k];
        }
        a2435_z[m] = a2435_cur[m];
        tmp = a2435_prev;
        a2435_prev = a2435_cur;
        a2435_cur = tmp;
        a2435_rows++;
    }
}

static seq_t a2435_fixed(idx_t n) {
    idx_t K = (n % 2 == 0) ? 2 : 3;
    idx_t m, k;
    seq_t *cur = a2435_t0, *nxt = a2435_t1, *tmp;
    if (K > n) return 0;
    cur[0] = 1;
    for (m = 0; m < n; m++) {
        idx_t lim = K + n - m - 1;
        idx_t hi = (m + 1 < lim) ? m + 1 : lim;
        for (k = 0; k <= hi; k++) {
            seq_t v = 0;
            if (k + 1 <= m) v += (seq_t)(k + 1) * cur[k + 1];
            if (k >= 1) v += (seq_t)k * cur[k - 1];
            nxt[k] = v;
        }
        tmp = cur;
        cur = nxt;
        nxt = tmp;
    }
    return cur[K];
}

seq_t a(idx_t n) {
    if (a2435_is_fixed() || n + 3 > A2435_MAXN)
        return a2435_fixed(n);
    a2435_extend(n + 3);
    if (n % 2 == 0)
        return (a2435_z[n + 2] - a2435_z[n]) / 2;
    return (a2435_z[n + 3] - 5 * a2435_z[n + 1]) / 6;
}
