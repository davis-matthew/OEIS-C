/*
 * A115115  Number of 3-asymmetric rhythm cycles: binary necklaces of length
 *          3n subject to the restriction that for any k if the k-th bead is
 *          of color 1 then the (k+n)-th and (k+2n)-th beads (modulo 3n) are
 *          of color 0.
 * https://oeis.org/A115115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (∑_{d|n} φ(3d) + ∑_{d|n, (3,d)=1} φ(d)·4^{n/d}) / (3n)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   phi, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    seq_t numer = 0;
    for (idx_t d = 1; d <= n; ++d) {
        if (n % d == 0) {
            numer += phi(3 * d);          /* first sum: φ(3d) */
            if (d % 3 != 0) {              /* second sum only if (3,d)=1 */
                seq_t phid = phi(d);
                seq_t pow4_nd = pow4(n / d);   /* 4^{n/d} */
                numer += phid * pow4_nd;
            }
        }
    }
    seq_t denom = (seq_t)3 * (seq_t)n;
    return numer / denom;
}
