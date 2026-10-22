/*
 * A006439  Number of tree-rooted toroidal maps with 2 faces and n vertices
 *          and without separating loops.
 * https://oeis.org/A006439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   with m = n-1 and Cat(k) = binomial(2k,k)/(k+1), a(n) =
 *            10*C(2m+6,6)*Cat(m) - Sum_{u=0..m} Cat(u)*Cat(m-u)*(
 *            5*C(2(m-u)+5,5) + (2(m-u)+1)*C(2u+4,4) ). A tree-rooted map is a
 *            shuffle of a Dyck word (the tree) with a rooted genus-1
 *            one-vertex map with 3 edges (10 of these). The sum subtracts the
 *            shu...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   a006439_bn
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a006439_bn.h"

seq_t a(idx_t n)
{
    idx_t m = n - 1;
    idx_t k, u;
    seq_t c = 1, total, B = 0, T;

    for (k = 0; k < m; k++) {
        c = c * (seq_t)(2 * (2 * k + 1));
        c /= (seq_t)(k + 2);
    }
    total = (seq_t)10 * a006439_bn(2 * m + 6, 6) * c;

    T = c; /* T = Cat(u)*Cat(m-u) */
    for (u = 0; u <= m; u++) {
        idx_t v = m - u;
        seq_t w = (seq_t)5 * a006439_bn(2 * v + 5, 5)
                + (seq_t)(2 * v + 1) * a006439_bn(2 * u + 4, 4);
        B += T * w;
        if (u < m) {
            T = T * (seq_t)(2 * u + 1);
            T = T * (seq_t)(m - u + 1);
            T /= (seq_t)(u + 2);
            T /= (seq_t)(2 * m - 2 * u - 1);
        }
    }
    return total - B;
}
