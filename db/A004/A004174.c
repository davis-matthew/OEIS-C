/*
 * A004174  Triangle of coefficients of Euler polynomials 2^n*E_n(x)
 *          (exponents in increasing order).
 * https://oeis.org/A004174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, nice
 * Formula:   T(n,k) = binomial(n,k) * 2^k * f(n-k), where f(j) = 2^j*E_j(0):
 *            f(0) = 1, f(2m) = 0 for m >= 1, f(2m-1) = (-1)^m * A000182(m)
 *            (tangent numbers, computed as zigzag numbers A000111(2m-1) via
 *            the Seidel-Entringer boustrophedon: row 0 = [1], E(i,0) = 0,
 *            E(i,t) = E(i,t-1) + E(i-1,i-t), zigzag(i) = E(i...
 * Range:     i8 n <= 21, i16 n <= 46, i32 n <= 121, i64 n <= 300,
 *            i128 n <= 779, big any n
 * Helpers:   a000037_isqrt, alloc_zero, binom, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/a000037_isqrt.h"
#include "../utils/binom.h"
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t zigzag(idx_t j) {
    seq_t *prev = alloc_zero(j + 1);
    seq_t *cur = alloc_zero(j + 1);
    seq_t res;
    idx_t i, t;
    prev[0] = 1;
    for (i = 1; i <= j; i++) {
        cur[0] = 0;
        for (t = 1; t <= i; t++)
            cur[t] = cur[t - 1] + prev[i - t];
        for (t = 0; t <= i; t++)
            prev[t] = cur[t];
    }
    res = prev[j];
    release(prev, j + 1);
    release(cur, j + 1);
    return res;
}

seq_t a(idx_t n) {
    idx_t r = (a000037_isqrt(8 * n + 1) - 1) / 2;
    idx_t k, j, i;
    seq_t f, res;
    while (r > 0 && r * (r + 1) / 2 > n) r--;
    while ((r + 1) * (r + 2) / 2 <= n) r++;
    k = n - r * (r + 1) / 2;
    j = r - k;
    if (j == 0) {
        f = 1;
    } else if (j % 2 == 0) {
        return 0;
    } else {
        idx_t m = (j + 1) / 2;
        f = zigzag(j);
        if (m % 2 == 1) f = -f;
    }
    res = f * binom(r, k);
    for (i = 0; i < k; i++) res = res * 2;
    return res;
}
