/*
 * A003443  Number of nonequivalent dissections of an n-gon into n-4 polygons
 *          by nonintersecting diagonals rooted at a cell up to rotation.
 * https://oeis.org/A003443
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ( C(n-3,2)*C(2n-6,n-5) + [5|n] * 4*(n/5)*A000108(n/5-1) )
 *            / n (Burnside over the n rotations; only the order-5 rotations
 *            fix a rooted dissection: central pentagon with identical
 *            triangulated flaps)
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 19, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   a3443_binom, a3443_catalan, a3443_g
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a3443_binom.h"
#include "../utils/a3443_catalan.h"
#include "../utils/a3443_g.h"

seq_t a(idx_t n) {
    uint64_t P, rB, small;
    seq_t B, qB, Q = 0, sn;
    uint64_t R = 0;
    if (n < 5) return 0;
    sn = (seq_t)n;
    if (sn == 0) return 0;
    P = (uint64_t)(n - 3) * (uint64_t)(n - 4) / 2;
    B = a3443_binom(2 * n - 6, n - 5);
    qB = B / sn;
    rB = (uint64_t)(B % sn);
    if (n % 5 == 0) {
        seq_t C = a3443_catalan(n / 5 - 1);
        uint64_t c5 = (uint64_t)(C % 5);
        Q = (C / 5) * 4 + (seq_t)((c5 * 4) / 5);
        R = (c5 * 4) % 5;
    }
    small = P * rB + (n / 5) * R;
    return (seq_t)P * qB + Q + (seq_t)(small / n);
}
