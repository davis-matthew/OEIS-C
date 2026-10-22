/*
 * A001975  Number of partitions of floor(5n/2) into n nonnegative integers
 *          each no more than 5.
 * https://oeis.org/A001975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f.: -(x^14 -x^13 +2*x^12 +x^11 +2*x^10 +3*x^9 +x^8 +5*x^7 +x^6
 *            +3*x^5 +2*x^4 +x^3 +2*x^2 -x+1) /
 *            ((x^4+1)*(x^2+x+1)*(x^2-x+1)*(x^2+1)^2*(x+1)^3*(x-1)^5);
 *            implemented as the order-20 linear recurrence a(n) = N_n +
 *            Sum_{k=1..20} D_k*a(n-k), where N is the numerator and D the
 *            denominator polynomia...
 * Range:     u8 n <= 12, u16 n <= 57, u32 n <= 960, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a001975_pmul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a001975_pmul.h"

seq_t a(idx_t n) {
    static const long long N[15] = {1, -1, 2, 1, 2, 3, 1, 5, 1, 3, 2, 1, 2, -1, 1};
    long long D[32];
    int deg = 0, t;
    const long long f1m[2] = {1, -1};
    const long long f1p[2] = {1, 1};
    const long long f2p[3] = {1, 0, 1};
    const long long f4p[5] = {1, 0, 0, 0, 1};
    const long long f421[5] = {1, 0, 1, 0, 1};
    seq_t w[21];
    seq_t res = 0;
    idx_t i;

    D[0] = 1;
    for (t = 0; t < 5; t++) a001975_pmul(D, &deg, f1m, 1);
    for (t = 0; t < 3; t++) a001975_pmul(D, &deg, f1p, 1);
    for (t = 0; t < 2; t++) a001975_pmul(D, &deg, f2p, 2);
    a001975_pmul(D, &deg, f4p, 4);
    a001975_pmul(D, &deg, f421, 4);
    for (t = 0; t <= deg; t++) D[t] = -D[t];   /* (x-1)^5 = -(1-x)^5 */

    for (i = 0; i <= n; i++) {
        seq_t pos = 0, neg = 0;
        idx_t k, kmax;
        if (i < 15) {
            if (N[i] > 0) pos += (seq_t)N[i];
            else neg += (seq_t)(-N[i]);
        }
        kmax = i < (idx_t)deg ? i : (idx_t)deg;
        for (k = 1; k <= kmax; k++) {
            long long c = D[k];
            seq_t v = w[(i - k) % 21];
            if (c > 0) pos += v * (seq_t)c;
            else if (c < 0) neg += v * (seq_t)(-c);
        }
        res = pos - neg;
        w[i % 21] = res;
    }
    return res;
}
