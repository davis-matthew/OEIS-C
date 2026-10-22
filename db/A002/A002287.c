/*
 * A002287  Bisection of A002470.
 * https://oeis.org/A002287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, more
 * Formula:   a(1) = 0; for n >= 2, a(n) = c(n-1), where Sum_{k>=1} c(k) q^k =
 *            psi(q) * (4*theta_3(q)^4 - 64*Sum_{m odd} sigma(m) q^m), psi(q)
 *            = eta(q)^4 eta(q^2)^2 eta(q^4)^4 = (1/4) Sum_{x^2+y^2=j}
 *            Re((x+iy)^4) q^j. So c(k) = Sum_{j=1..k} psi(j) E(k-j), with
 *            E(0) = 4, E(m) = -32 sigma(m) for odd m, E(m) = 96...
 * Range:     i8 n <= 3, i16 n <= 16, i32 n <= 36, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    long long k, x, y, j, d, m;
    long long *psi, *sig;
    seq_t s = 0;
    if (n < 2) return 0;
    k = (long long)(n - 1);
    psi = (long long *)calloc((size_t)(k + 1), sizeof(long long));
    sig = (long long *)calloc((size_t)(k + 1), sizeof(long long));

    for (x = 0; x * x <= k; x++) {
        for (y = 0; x * x + y * y <= k; y++) {
            long long v, mult;
            j = x * x + y * y;
            if (j == 0) continue;
            v = x * x * x * x - 6 * x * x * y * y + y * y * y * y;
            mult = (x ? 2 : 1) * (y ? 2 : 1);
            psi[j] += mult * v;
        }
    }
    for (j = 1; j <= k; j++) psi[j] /= 4;

    for (d = 1; d <= k; d++)
        for (m = d; m <= k; m += d) sig[m] += d;

    for (j = 1; j <= k; j++) {
        long long e;
        if (psi[j] == 0) continue;
        m = k - j;
        if (m == 0) {
            e = 4;
        } else if (m & 1) {
            e = -32 * sig[m];
        } else {
            long long o = m;
            while ((o & 1) == 0) o >>= 1;
            e = 96 * sig[o];
        }
        s += (seq_t)(psi[j] * e);
    }

    free(psi);
    free(sig);
    return s;
}
