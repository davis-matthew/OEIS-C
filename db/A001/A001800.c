/*
 * A001800  Coefficients of Legendre polynomials.
 * https://oeis.org/A001800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1) * C(2n+2, n+1) / 2^A000120(n+2); computed as
 *            (n+1)*2^(v2(n+2)-1) * odd part of C(2n+2,n+1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   a001800_g
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a001800_g.h"

seq_t a(idx_t n) {
    idx_t m = n + 1;
    idx_t k;
    seq_t O = 1; /* odd part of C(2k,k) */
    for (k = 1; k <= m; k++) {
        uint64_t d = k;
        uint64_t f = 2 * k - 1;
        uint64_t r, g;
        while ((d & 1) == 0) d >>= 1;
        if (d > 1) {
            r = (uint64_t)(O % (seq_t)d);
            g = a001800_g(d, r);
            O /= (seq_t)g;
            d /= g;
            f /= d;
        }
        O *= (seq_t)f;
    }
    {
        uint64_t t;
        uint64_t q = n + 2;
        if ((m & 1) == 0) {
            t = m / 2;
        } else {
            t = m;
            q >>= 1;
            while ((q & 1) == 0) {
                t <<= 1;
                q >>= 1;
            }
        }
        return O * (seq_t)t;
    }
}
