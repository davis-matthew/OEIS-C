/*
 * A005723  Quadrinomial coefficients.
 * https://oeis.org/A005723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^(3n-2)] (1+x+x^2+x^3)^(2n-1) = Sum_{j} binomial(2n-1,
 *            j) * binomial(2n-1, 3n-2-2j)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 19 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"

static int seq_is_big(void)
{
    seq_t x = 1;
    int i;
    for (i = 0; i < 130; i++) x = x * 2;
    return !(x == 0);
}

seq_t a(idx_t n) {
    idx_t m, K, j0, j1, j;
    seq_t s = 0;
    if (n < 1) return 0;
    m = 2 * n - 1;
    K = 3 * n - 2;
    j0 = n / 2;      /* smallest j with K - 2j <= m */
    j1 = K / 2;      /* largest j with K - 2j >= 0 */

    if (!seq_is_big()) {
        for (j = j0; j <= j1; j++)
            s += binomial(m, j) * binomial(m, K - 2 * j);
        return s;
    }

    {
        seq_t A = binomial(m, j0);
        seq_t B = binomial(m, K - 2 * j0);
        for (j = j0;; j++) {
            idx_t r;
            s += A * B;
            if (j == j1) break;
            r = K - 2 * j;
            A = A * (m - j) / (j + 1);
            B = B * r / (m - r + 1);
            B = B * (r - 1) / (m - r + 2);
        }
    }
    return s;
}
