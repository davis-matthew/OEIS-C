/*
 * A005874  Theta series of hexagonal close-packing with respect to triangle
 *          between tetrahedra.
 * https://oeis.org/A005874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{m even, 2m^2<=n, n-2m^2 == 1 (mod 3)} 3*E(n-2m^2) +
 *            Sum_{m odd, 2m^2<=n, n-2m^2 == 0 (mod 3)} R((n-2m^2)/3), with
 *            E(k) = Sum_{d|k} Kronecker(-3,d), R(0)=1 and R(k)=6*E(k) for
 *            k>=1. Norms are 3*|x|^2. The h.c.p. has nearest-neighbor
 *            distance 1 and layer spacing sqrt(2/3). The center si...
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   eis_E
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/eis_E.h"

seq_t a(idx_t n) {
    unsigned long long N = (unsigned long long)n;
    unsigned long long total = 0;
    long long m;
    for (m = 0; (unsigned long long)(2 * m * m) <= N; m++) {
        unsigned long long k = N - (unsigned long long)(2 * m * m);
        unsigned long long c = 0;
        if (m % 2 == 0) {
            if (k % 3 == 1) c = (unsigned long long)(3 * eis_E(k));
        } else {
            if (k % 3 == 0) {
                if (k == 0) c = 1;
                else c = (unsigned long long)(6 * eis_E(k / 3));
            }
        }
        total += (m == 0) ? c : 2 * c;
    }
    return (seq_t)total;
}
