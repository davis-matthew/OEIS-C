/*
 * A006027  Number of directed column-convex polyominoes with perimeter 2n+2.
 * https://oeis.org/A006027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   the g.f. u(x) = Sum_{n>=1} a(n)*x^n satisfies u^3 - (x+2)*u^2 +
 *            (x^2+x+1)*u - x = 0 (D. G. Rogers's equation with u = A + x), so
 *            a(n) = 2*S(n) + S(n-1) - T(n) - a(n-1) - a(n-2) + [n=1], where
 *            S(n) = [x^n] u^2, T(n) = [x^n] u^3 and a(0) = 0, a(-1) = 0
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 18, u64 n <= 33, u128 n <= 63,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/grow_zero.h"

static seq_t *va = 0;   /* va[k] = a(k), va[0] = 0 */
static seq_t *vs = 0;   /* vs[k] = [x^k] u^2 */
static idx_t cap = 0;
static idx_t done = 0;  /* va[0..done-1] are computed */

seq_t a(idx_t n)
{
    if (n >= cap) {
        idx_t nc = cap ? cap : 64;
        while (nc <= n) nc *= 2;
        va = grow_zero(va, cap, nc);
        vs = grow_zero(vs, cap, nc);
        cap = nc;
    }
    if (done == 0) done = 1;
    while (done <= n) {
        idx_t m = done;
        idx_t i;
        seq_t s = 0, t = 0, p, q;

        /* S(m) = Sum_{i=1}^{m-1} a(i) a(m-i) */
        for (i = 1; 2 * i < m; i++) s += va[i] * va[m - i];
        s += s;
        if (m % 2 == 0) s += va[m / 2] * va[m / 2];
        vs[m] = s;

        /* T(m) = Sum_{i=1}^{m-2} a(i) S(m-i) */
        for (i = 1; i + 2 <= m; i++) t += va[i] * vs[m - i];

        p = s + s + vs[m - 1];
        if (m == 1) p += 1;
        q = t + va[m - 1];
        if (m >= 2) q += va[m - 2];
        va[m] = p - q;
        done++;
    }
    return va[n];
}
