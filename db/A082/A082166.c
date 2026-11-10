/*
 * A082166  a(n) = A006689(n)/n^2.
 * https://oeis.org/A082166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{0<=f_1<2, f_{i-1}<f_i<2i} (Product_{i=1..n-1} i^(f_i
 *            - f_{i-1} - 1)) * n^(2n - f_{n-1} - 3); a(1) = 1. This is
 *            A006689(n)/n^2, computed by DP over (i, f_i)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 19 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;

    idx_t len = 2 * n;
    seq_t *D = mem_new(seq_t, (size_t)len);
    D[0] = 1;
    D[1] = 1;

    for (idx_t i = 2; i + 1 <= n; i++) {
        seq_t S = 0;
        idx_t lim = 2 * i;
        for (idx_t f = 0; f < lim; f++) {
            seq_t t = D[f];
            D[f] = S;
            S = S * (seq_t)i + t;
        }
    }

    seq_t acc = 0;
    for (idx_t f = 0; f + 3 <= len; f++) {
        acc = acc * (seq_t)n + D[f];
    }

    mem_free(D);
    return acc;
}
