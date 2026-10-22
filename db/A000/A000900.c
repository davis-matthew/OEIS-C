/*
 * A000900  Number of solutions to the rook problem on an n X n board having a
 *          certain symmetry group (see Robinson for details).
 * https://oeis.org/A000900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (A000085(n) - A000898(floor(n/2)))/2, computed as a(n) =
 *            u(n) - v(floor(n/2)) with u(n) = A000085(n)/2: u(2) = 1, u(3) =
 *            2, u(n) = u(n-1) + (n-1)*u(n-2); and v(m) = A000898(m)/2: v(1) =
 *            1, v(2) = 3, v(m) = 2*v(m-1) + 2*(m-1)*v(m-2); a(0) = a(1) = 0
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 32, u128 n <= 54,
 *            big any n
 * Helpers:   half_a000085, half_a000898
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/half_a000085.h"
#include "../utils/half_a000898.h"

seq_t a(idx_t n) {
    if (n < 2) return 0;
    return half_a000085(n) - half_a000898(n / 2);
}
