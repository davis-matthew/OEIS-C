/*
 * A005508  Number of unrooted triangulations with reflection symmetry of a
 *          disk with one internal node and n+3 nodes on the boundary.
 * https://oeis.org/A005508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   with C(m) = binomial(2m,m)/(m+1) (Catalan numbers), a(2m) =
 *            C(m)*(7m+2)/(m+2), a(2m-1) = 6m*C(m)/(m+2)
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 36, u64 n <= 69, u128 n <= 134,
 *            big any n
 * Helpers:   muldiv, u64_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/muldiv.h"
#include "../utils/u64_gcd.h"

static seq_t catalan(uint64_t m) {
    seq_t c = 1;
    uint64_t k;
    for (k = 0; k < m; k++)
        c = muldiv(c, 2 * (2 * k + 1), k + 2);
    return c;
}

seq_t a(idx_t n) {
    uint64_t m;
    seq_t c;
    if (n % 2 == 0) {
        m = n / 2;
        c = catalan(m);
        return muldiv(c, 7 * m + 2, m + 2);
    } else {
        m = (n + 1) / 2;
        c = catalan(m);
        return muldiv(c, 6 * m, m + 2);
    }
}
