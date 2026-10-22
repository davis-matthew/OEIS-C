/*
 * A002612  Glaisher's function U(n).
 * https://oeis.org/A002612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   G.f. = theta_3(q)^8 * eta(q)^4*eta(q^2)^2*eta(q^4)^4, i.e. a(n)
 *            = Sum_{k=1..n} c(k)*A000143(n-k), where c(k) =
 *            (1/4)*Sum_{x^2+y^2=k} (x^4 - 6*x^2*y^2 + y^4) (coeffs of q*Prod
 *            (1-q^j)^4 (1-q^2j)^2 (1-q^4j)^4) and A000143(m) = r_8(m) =
 *            16*Sum_{d|m} (-1)^(m+d)*d^3, A000143(0) = 1
 * Range:     i8 n <= 4, i16 n <= 17, i32 n <= 196, i64 n <= 500,
 *            i128 n <= 500, big any n
 * Helpers:   r8_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/r8_val.h"

seq_t a(idx_t n)
{
    long long N = (long long)n;
    long long R = 0, x, y, k;
    long long *f;
    seq_t total = 0;

    while ((R + 1) * (R + 1) <= N) R++;
    f = (long long *)calloc((size_t)(N + 1), sizeof(long long));
    for (x = -R; x <= R; x++) {
        for (y = -R; y <= R; y++) {
            long long q = x * x + y * y;
            if (q > N) continue;
            f[q] += x * x * x * x - 6 * x * x * y * y + y * y * y * y;
        }
    }
    for (k = 1; k <= N; k++) {
        if (f[k] != 0) {
            seq_t c = (seq_t)(f[k] / 4);
            total += c * r8_val(N - k);
        }
    }
    free(f);
    return total;
}
