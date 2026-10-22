/*
 * A001869  Number of n-bead necklaces with 5 colors.
 * https://oeis.org/A001869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/n)*Sum_{d|n} phi(d)*5^(n/d), n > 0; a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   a1869_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a1869_phi.h"

seq_t a(idx_t n) {
    seq_t q, Q;
    uint64_t r, R, e;
    if (n == 0) return 1;
    /* 5^e = q*n + r, kept as quotient/remainder so the sum is divided exactly */
    q = (seq_t)(1 / n);
    r = 1 % n;
    Q = 0;
    R = 0;
    for (e = 1; e <= n; e++) {
        uint64_t r5 = r * 5;
        q = q * 5 + (seq_t)(r5 / n);
        r = r5 % n;
        if (n % e == 0) {
            uint64_t ph = a1869_phi(n / e);
            uint64_t rp = r * ph;
            Q += q * (seq_t)ph + (seq_t)(rp / n);
            R += rp % n;
            if (R >= n) {
                R -= n;
                Q += 1;
            }
        }
    }
    return Q;
}
