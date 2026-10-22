/*
 * A000731  Expansion of Product (1 - x^k)^8 in powers of x.
 * https://oeis.org/A000731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   G.f. = eta(q)^8 = (eta^3 * eta)^2, where eta^3 = Sum_{m>=0}
 *            (-1)^m (2m+1) x^(m(m+1)/2) (Jacobi) and eta = Sum_k (-1)^k
 *            x^(k(3k-1)/2) (Euler pentagonal); a(n) = Sum_{i=0..n}
 *            e4(i)*e4(n-i), e4 = eta^4 coefficients
 * Range:     i8 n <= 8, i16 n <= 185, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    seq_t *e4 = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    idx_t i, m, j;
    seq_t result;
    for (i = 0; i <= n; i++) e4[i] = 0;

    for (m = 0; m * (m + 1) / 2 <= n; m++) {
        idx_t T = m * (m + 1) / 2;
        long long c = (long long)(2 * m + 1);
        if (m & 1) c = -c;
        for (j = 0; ; j++) {
            idx_t g1 = j * (3 * j - (j > 0 ? 1 : 0)) / 2; /* k = j: j(3j-1)/2 */
            idx_t g2 = j * (3 * j + 1) / 2;               /* k = -j: j(3j+1)/2 */
            long long s = (j & 1) ? -c : c;
            if (j == 0) g1 = 0;
            if (T + g1 > n) break;
            e4[T + g1] += (seq_t)s;
            if (j > 0 && T + g2 <= n) e4[T + g2] += (seq_t)s;
        }
    }

    result = 0;
    for (i = 0; i <= n; i++) {
        if (e4[i] != 0 && e4[n - i] != 0)
            result += e4[i] * e4[n - i];
    }
    free(e4);
    return result;
}
