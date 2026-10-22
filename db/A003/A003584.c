/*
 * A003584  Unicursal (i.e., possessing an Eulerian path) planar rooted maps
 *          with n edges.
 * https://oeis.org/A003584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = A000257(n) + A069720(n) =
 *            2^(n-2)*C(2n,n)*(n^2+3n+8)/((n+1)(n+2)) for n >= 1, a(0) = 1;
 *            computed by a(n) = a(n-1) * 4(2n-1)(n^2+3n+8) /
 *            ((n+2)(n^2+n+6)), a(1) = 2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   a3584_g
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/a3584_g.h"

seq_t a(idx_t n) {
    seq_t v;
    idx_t k;
    if (n == 0) return (seq_t)1;
    v = (seq_t)2;
    for (k = 2; k <= n; k++) {
        uint64_t num[3], den[2];
        int i, j;
        num[0] = 4;
        num[1] = 2 * (uint64_t)k - 1;
        num[2] = (uint64_t)k * k + 3 * (uint64_t)k + 8;
        den[0] = (uint64_t)k + 2;
        den[1] = (uint64_t)k * k + (uint64_t)k + 6;
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 2; j++) {
                uint64_t g = a3584_g(num[i], den[j]);
                num[i] /= g;
                den[j] /= g;
            }
        }
        /* now num and den are coprime, so den divides v */
        v = v / (idx_t)den[0];
        v = v / (idx_t)den[1];
        v = v * (idx_t)num[0];
        v = v * (idx_t)num[1];
        v = v * (idx_t)num[2];
    }
    return v;
}
