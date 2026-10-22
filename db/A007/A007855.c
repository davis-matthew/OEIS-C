/*
 * A007855  Infima closed sets in rooted plane trees on n nodes.
 * https://oeis.org/A007855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = Sum_{m=1..n} t(m)*binomial(2n-2, n-m), where Sum_{m>=1}
 *            t(m) w^m = ((1-w) - sqrt((1-w)(1-5w)))/2, i.e. t(1) = 1, t(2) =
 *            1, m*t(m) = (6m-9)*t(m-1) - (5m-15)*t(m-2) for m >= 3. (Lagrange
 *            inversion of the g.f.
 *            (1/4)(1+sqrt(1-4x))(1-sqrt(3-2/sqrt(1-4x))) with x = w/(1+w)^2.)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/grow_zero.h"

static seq_t *T = NULL;
static idx_t Tlen = 0;

static void ensure_t(idx_t n) {
    idx_t newlen, m;
    if (n < Tlen) return;
    newlen = n + 1;
    if (newlen < 2 * Tlen) newlen = 2 * Tlen;
    if (newlen < 3) newlen = 3;
    T = grow_zero(T, Tlen, newlen);
    if (Tlen < 3) {
        T[0] = 0;
        T[1] = 1;
        T[2] = 1;
        Tlen = 3;
    }
    for (m = Tlen; m < newlen; m++) {
        seq_t x = (seq_t)(6 * m - 9) * T[m - 1];
        seq_t y = (seq_t)(5 * m - 15) * T[m - 2];
        T[m] = (x - y) / (seq_t)m;
    }
    Tlen = newlen;
}

seq_t a(idx_t n) {
    idx_t N, j;
    seq_t b, s;
    if (n == 0) return 0;
    ensure_t(n);
    N = 2 * n - 2;
    b = 1;
    s = 0;
    for (j = 0; j < n; j++) {
        s += T[n - j] * b;
        if (j + 1 < n) {
            b = b * (seq_t)(N - j);
            b = b / (seq_t)(j + 1);
        }
    }
    return s;
}
