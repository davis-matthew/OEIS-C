/*
 * A003446  Number of nonequivalent dissections of a polygon into n triangles
 *          by nonintersecting diagonals rooted at a cell up to rotation and
 *          reflection.
 * https://oeis.org/A003446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (C(n+1) - C(n) + 3*[n even]*C(n/2) + 3*[n odd]*C((n-1)/2)
 *            + 2*[n == 1 mod 3]*C((n-1)/3)) / 6 for n >= 1, a(0) = 0, where
 *            C(k) = A000108(k) is the k-th Catalan number; this is the
 *            coefficient of z^n in (G(z)^2 - G(z) + 3*G(z^2) - 3 + 3z*G(z^2)
 *            + 2z*G(z^3)) / 6 with G(z) = 1 + z*G(z)^2. Exac...
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 20, u64 n <= 36, u128 n <= 69,
 *            big any n
 * Helpers:   a3446_add, a3446_catalan, a3446_div, a3446_mul, a3446_sub
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/a3446_add.h"
#include "../utils/a3446_catalan.h"
#include "../utils/a3446_div.h"
#include "../utils/a3446_mul.h"
#include "../utils/a3446_sub.h"

seq_t a(idx_t n) {
    size_t cap, xl, tl, i;
    uint32_t *X, *T;
    seq_t r;
    if (n == 0) return 0;

    cap = (size_t)((2 * n + 128) / 32 + 4);
    X = (uint32_t *)calloc(cap, sizeof(uint32_t));
    T = (uint32_t *)calloc(cap, sizeof(uint32_t));

    /* X = C(n+1) - C(n) */
    a3446_catalan(n, X, &xl);
    memcpy(T, X, xl * sizeof(uint32_t));
    tl = xl;
    a3446_mul(X, &xl, (uint32_t)(2 * (2 * n + 1)));
    a3446_div(X, &xl, (uint32_t)(n + 2));
    a3446_sub(X, &xl, T, tl);

    /* + 3*C(floor(n/2)) : from 3*G(z^2) (n even) or 3z*G(z^2) (n odd) */
    a3446_catalan(n / 2, T, &tl);
    a3446_mul(T, &tl, 3);
    a3446_add(X, &xl, T, tl);

    /* + 2*C((n-1)/3) when n == 1 mod 3 : from 2z*G(z^3) */
    if (n % 3 == 1) {
        a3446_catalan((n - 1) / 3, T, &tl);
        a3446_mul(T, &tl, 2);
        a3446_add(X, &xl, T, tl);
    }

    a3446_div(X, &xl, 6);

    r = 0;
    i = xl;
    while (i-- > 0) {
        r <<= 8; r <<= 8; r <<= 8; r <<= 8;
        r += (seq_t)X[i];
    }
    free(X);
    free(T);
    return r;
}
