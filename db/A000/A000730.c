/*
 * A000730  Expansion of Product_{n>=1} (1 - x^n)^7.
 * https://oeis.org/A000730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k} (-1)^k * c(n - k*(3k-1)/2), k over all integers
 *            (Euler's pentagonal theorem), where c(m) = Sum_{T_i + T_j = m}
 *            (-1)^(i+j) (2i+1)(2j+1), T_i = i*(i+1)/2, is the coefficient of
 *            x^m in Product_{n>=1} (1 - x^n)^6 (the square of Jacobi's
 *            identity Product (1-x^n)^3 = Sum_{i>=0} (-1)^i (2...
 * Range:     i8 n <= 8, i16 n <= 608, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   eta6_coef, tri_u
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/eta6_coef.h"
#include "../utils/tri_u.h"

seq_t a(idx_t n) {
    long long s = eta6_coef(n);
    uint64_t k;
    for (k = 1;; k++) {
        uint64_t g1 = k * (3 * k - 1) / 2;
        uint64_t g2 = k * (3 * k + 1) / 2;
        long long t = 0;
        if (g1 > n) break;
        t += eta6_coef(n - g1);
        if (g2 <= n) t += eta6_coef(n - g2);
        s += (k & 1) ? -t : t;
    }
    return (seq_t)s;
}
