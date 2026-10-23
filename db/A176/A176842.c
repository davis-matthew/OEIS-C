/*
 * A176842  The number of iterations of the map x -> x - phi(bigomega(x))
 *          needed to reach 1 starting at x=n.
 * https://oeis.org/A176842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of iterations of the map x -> x - phi(bigomega(x))
 *            needed to reach 1 starting at x = n
 * Range:     u8 n <= 522, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bigomega.h"
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; return 0 for safety */
        return 0;

    seq_t   steps = 0;
    idx_t   x     = n;

    while (x > 1) {
        /* bigomega returns seq_t, but its value fits in idx_t (≤64 for 64‑bit n) */
        seq_t   omega_seq   = bigomega(x);
        idx_t   omega       = (idx_t)omega_seq;

        seq_t   phi_seq     = phi(omega);          /* phi expects idx_t, returns seq_t */
        idx_t   phi_val     = (idx_t)phi_seq;

        x -= phi_val;           /* next iterate of the map */
        ++steps;
    }

    return steps;
}
