/*
 * A004012  Theta series of hexagonal close-packing.
 * https://oeis.org/A004012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   G.f.: Sum_{i, j, k in Z} x^(8*i^2 + 3*(j^2 + j*k + k^2)) * (1 +
 *            x^(3 + 8*i + 3*j + 3*k)); a(n) counts the lattice terms with
 *            exponent n by direct search
 * Range:     u8 n <= 1190, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   a000037_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a000037_isqrt.h"

seq_t a(idx_t n) {
    seq_t count = 0;
    int64_t N = (int64_t)n;
    int64_t r = (int64_t)a000037_isqrt((uint64_t)n);
    int64_t mi = r + 2;
    int64_t mjk = r + 3;
    int64_t i, j, k;

    for (i = -mi; i <= mi; i++) {
        int64_t e1 = 8 * i * i;
        int64_t e2 = 8 * i * i + 8 * i + 3;
        int ok1 = e1 <= N;
        int ok2 = e2 - 3 <= N;
        if (!ok1 && !ok2) continue;
        for (j = -mjk; j <= mjk; j++) {
            for (k = -mjk; k <= mjk; k++) {
                int64_t q = 3 * (j * j + j * k + k * k);
                if (ok1 && e1 + q == N) count++;
                if (ok2 && e2 + q + 3 * j + 3 * k == N) count++;
            }
        }
    }
    return count;
}
