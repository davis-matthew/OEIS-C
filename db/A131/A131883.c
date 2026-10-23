/*
 * A131883  a(n) = the minimum value from among
 *          (phi(n+1),phi(n+2),phi(n+3),...,phi(2n)), where phi(m) (A000010)
 *          is the number of positive integers which are coprime to m and are
 *          <= m.
 * https://oeis.org/A131883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 190, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = minimum of φ(n+1), φ(n+2), … , φ(2n)  (φ = Euler's totient) */

#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* start at n+1 (first member of the interval) */
    idx_t start = n + 1;
    /* end at 2n (inclusive) */
    idx_t end   = 2 * n;

    /* initialise best with the first value in the interval */
    seq_t best = phi(start);

    /* scan the rest of the interval */
    for (idx_t m = start + 1; m <= end; ++m) {
        seq_t v = phi(m);
        if (v < best)
            best = v;
    }

    return best;
}
