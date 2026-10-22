/*
 * A004175  Triangle of coefficients of Euler polynomials 2^n*E_n(x)
 *          (exponents in decreasing order).
 * https://oeis.org/A004175
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, nice, easy
 * Formula:   T(n,k) = coefficient of x^(n-k) in 2^n*E_n(x), read by rows:
 *            T(n,0) = 1, T(n,k) = 0 for even k > 0, T(n,k) = (-1)^((k+1)/2) *
 *            binomial(n,k) * A000182((k+1)/2) * 2^(n-k) for odd k, where
 *            A000182 are the tangent numbers; a(n) = T(r,k) with n =
 *            r*(r+1)/2 + k, 0 <= k <= r
 * Range:     i8 n <= 21, i16 n <= 51, i32 n <= 128, i64 n <= 316,
 *            i128 n <= 812, big any n
 * Helpers:   alloc_zero, binom, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binom.h"
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

/* tangent number A000182(m), m >= 1, Brent-Zimmermann recurrence */
static seq_t tangent(idx_t m) {
    seq_t *T = alloc_zero(m + 1);
    idx_t j, k;
    seq_t r;
    T[1] = 1;
    for (k = 2; k <= m; k++) T[k] = (seq_t)(k - 1) * T[k - 1];
    for (k = 2; k <= m; k++)
        for (j = k; j <= m; j++)
            T[j] = (seq_t)(j - k) * T[j - 1] + (seq_t)(j - k + 2) * T[j];
    r = T[m];
    release(T, m + 1);
    return r;
}

seq_t a(idx_t idx) {
    idx_t n = 0, k, i;
    seq_t c, r;
    while ((n + 1) * (n + 2) / 2 <= idx) n++;
    k = idx - n * (n + 1) / 2;

    if (k == 0) {
        c = 1;
    } else if (k % 2 == 0) {
        return 0;
    } else {
        idx_t m = (k + 1) / 2;
        c = tangent(m);
        if (m % 2 == 1) c = -c;
    }

    r = binom(n, k) * c;
    for (i = 0; i < n - k; i++) r *= 2;
    return r;
}
