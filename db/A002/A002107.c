/*
 * A002107  Expansion of Product_{k>=1} (1 - x^k)^2.
 * https://oeis.org/A002107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, nice, changed
 * Formula:   a(n) = Sum_{k=0..n} A010815(k)*A010815(n-k), where A010815(m) =
 *            (-1)^j if m = j*(3j-1)/2 (generalized pentagonal), else 0
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   a2107_eta, a2107_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a2107_eta.h"
#include "../utils/a2107_isqrt.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    /* enumerate generalized pentagonal numbers g = j(3j-1)/2, j = 0, 1, -1, 2, -2, ... */
    for (uint64_t j = 0;; j++) {
        uint64_t g1 = j * (3 * j - 1) / 2;      /* j >= 0 (j=0 gives 0) */
        if (j == 0) g1 = 0;
        if (g1 > n) break;
        int sg = (j & 1) ? -1 : 1;
        int e = a2107_eta(n - g1);
        if (e) sum += (seq_t)(sg * e);
        if (j > 0) {
            uint64_t g2 = j * (3 * j + 1) / 2;  /* j -> -j */
            if (g2 <= n) {
                e = a2107_eta(n - g2);
                if (e) sum += (seq_t)(sg * e);
            }
        }
    }
    return sum;
}
