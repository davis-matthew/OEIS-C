/*
 * A003723  E.g.f. exp(tanh(x)).
 * https://oeis.org/A003723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = P_n(0), where P_0(y) = 1 and P_{n+1}(y) = (1 - y^2) *
 *            (P_n'(y) + P_n(y)), since d^n/dx^n exp(tanh(x)) = P_n(tanh(x)) *
 *            exp(tanh(x))
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 14, i64 n <= 23, i128 n <= 38,
 *            big any n
 * Helpers:   alloc_zero, from_i128, release, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"
#include "../utils/from_i128.h"
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

seq_t a(idx_t n) {
    size_t len = (size_t)n + 2;
    if (seq_is_fixed_width()) {
        /* exact modulo 2^128 (ring operations only), so the result is
           correct whenever it fits in seq_t */
        unsigned __int128 *p = (unsigned __int128 *)calloc(len, sizeof(unsigned __int128));
        unsigned __int128 r;
        p[0] = 1;
        for (idx_t s = 1; s <= n; s++) {
            idx_t L = n - s;
            for (idx_t k = 0; k <= L; k++)
                p[k] += (unsigned __int128)(k + 1) * p[k + 1];
            for (idx_t k = L; k >= 2; k--)
                p[k] -= p[k - 2];
        }
        r = p[0];
        free(p);
        return from_i128((__int128)r);
    } else {
        seq_t *p = alloc_zero((idx_t)len);
        seq_t r;
        p[0] = 1;
        for (idx_t s = 1; s <= n; s++) {
            idx_t L = n - s;
            for (idx_t k = 0; k <= L; k++)
                p[k] += p[k + 1] * (seq_t)(k + 1);
            for (idx_t k = L; k >= 2; k--)
                p[k] -= p[k - 2];
        }
        r = p[0];
        release(p, (idx_t)len);
        return r;
    }
}
