/*
 * A007207  Magnetization for hexagonal lattice.
 * https://oeis.org/A007207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   n*a(n) = 10*(n-2)*a(n-2) - 6*a(n-3) - 9*(n-4)*a(n-4), a(0) = 1,
 *            a(1) = 0, a(2) = 0, a(3) = -2 (from G.f. G =
 *            ((1+x)^3*(1-3x)/((1-x)^3*(1+3x)))^(1/8), so (1-x^2)*(1-9x^2)*G'
 *            = -6*x^2*G)
 * Range:     i8 n <= 8, i16 n <= 14, i32 n <= 24, i64 n <= 30, i128 n <= 30,
 *            big any n
 * Helpers:   seq_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/seq_is_big.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n < 3) return 0;
    if (!seq_is_big()) {
        /* fixed-width build: use 128-bit intermediates */
        __int128 w0 = 0, w1 = 1, w2 = 0, w3 = 0; /* g(k-4), g(k-3), g(k-2), g(k-1) */
        __int128 gk = 0;
        for (idx_t k = 3; k <= n; k++) {
            __int128 kk = (__int128)k;
            gk = (10 * (kk - 2) * w2 - 6 * w1 - 9 * (kk - 4) * w0) / kk;
            w0 = w1; w1 = w2; w2 = w3; w3 = gk;
        }
        return (seq_t)gk;
    } else {
        seq_t w0 = 0, w1 = 1, w2 = 0, w3 = 0;
        seq_t gk = 0;
        for (idx_t k = 3; k <= n; k++) {
            seq_t kk = (seq_t)k;
            gk = (10 * (kk - 2) * w2 - 6 * w1 - 9 * (kk - 4) * w0) / kk;
            w0 = w1; w1 = w2; w2 = w3; w3 = gk;
        }
        return gk;
    }
}
