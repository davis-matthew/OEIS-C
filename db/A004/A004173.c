/*
 * A004173  Triangle of coefficients of Euler polynomials E_2n(x) (exponents
 *          in decreasing order).
 * https://oeis.org/A004173
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf, nice
 * Formula:   T(m,j) = coefficient of x^(2m-j) in E_2m(x) =
 *            binomial(2m,j)*E_j(0), with E_j(0) = G(j+1)/(j+1) (G = Genocchi
 *            numbers), so T(m,0) = 1, T(m,j) = 0 for even j > 0, T(m,2i-1) =
 *            binomial(2m,2i-1)*G(2i)/(2i); row m has 2m+1 entries, a(n) =
 *            T(m, n - m^2)
 * Range:     i8 n <= 31, i16 n <= 59, i32 n <= 135, i64 n <= 280,
 *            i128 n <= 665, big any n
 * Helpers:   alloc_zero, gcd_ll, release, vfact
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/gcd_ll.h"
#include "../utils/vfact.h"

/* |G(2i)| for i >= 1 via Seidel's triangle (all entries bounded by the result) */
static seq_t genocchi_abs(idx_t i) {
    idx_t len = 1, r, k;
    seq_t *t = alloc_zero(i + 1);
    seq_t g;
    t[0] = 1;
    for (r = 2; r <= 2 * i - 1; r++) {
        if (r % 2 == 0) {
            for (k = len - 1; k-- > 0;) t[k] += t[k + 1];
        } else {
            for (k = 1; k < len; k++) t[k] += t[k - 1];
            t[len] = t[len - 1];
            len++;
        }
    }
    g = t[len - 1];
    release(t, i + 1);
    return g;
}

seq_t a(idx_t n) {
    idx_t m = 0, j, i;
    long long N, J, q, d, p, e;
    seq_t g, B, res;
    while ((m + 1) * (m + 1) <= n) m++;
    j = n - m * m;
    if (j == 0) return 1;
    if (j % 2 == 0) return 0;
    i = (j + 1) / 2;
    g = genocchi_abs(i);
    d = gcd_ll((long long)(g % (seq_t)(2 * i)), (long long)(2 * i));
    q = (long long)(2 * i) / d;
    N = (long long)(2 * m);
    J = (long long)j;
    /* B = binomial(N, J) / q, built from prime powers so intermediates stay <= B */
    B = 1;
    for (p = 2; p <= N; p++) {
        long long t;
        bool pr = true;
        for (t = 2; t * t <= p; t++)
            if (p % t == 0) { pr = false; break; }
        if (!pr) continue;
        e = vfact(N, p) - vfact(J, p) - vfact(N - J, p);
        while (q % p == 0) { q /= p; e--; }
        while (e-- > 0) B *= (seq_t)p;
    }
    res = (g / (seq_t)d) * B;
    if (i % 2 == 1) res = -res;
    return res;
}
