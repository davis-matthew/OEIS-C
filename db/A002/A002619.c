/*
 * A002619  Number of 2-colored patterns on an n X n board.
 * https://oeis.org/A002619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = (1/n^2)*Sum_{d|n} phi(d)^2*(n/d)!*d^(n/d)
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 14, u64 n <= 22, u128 n <= 36,
 *            big any n
 * Helpers:   a2619_mul, a2619_phi, a2619_term
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a2619_mul.h"
#include "../utils/a2619_phi.h"
#include "../utils/a2619_term.h"

seq_t a(idx_t n) {
    uint64_t N = (uint64_t)n * (uint64_t)n;
    seq_t QS = 0;
    uint64_t RS = 0;
    idx_t d;
    for (d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            a2619_term(n, d, N, &QS, &RS);
            if (d * d != n) a2619_term(n, n / d, N, &QS, &RS);
        }
    }
    return QS + (seq_t)(RS / N);
}
