/*
 * A054571  a(n) = phi(n - phi(n)), a(1) = 0.
 * https://oeis.org/A054571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(n - phi(n))
 * Range:     u8 n <= 262, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi_calc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi_calc.h"

seq_t a(idx_t n)
{
    /* a(1) is defined as 0 */
    if (n <= 1)
        return 0;

    /* step 1: φ(n) */
    seq_t phi_n = phi_calc(n);

    /* step 2: n - φ(n)  (must stay ≥ 1 for n > 1) */
    seq_t m = (seq_t)n - phi_n;

    /* step 3: φ(n - φ(n)) */
    return phi_calc((idx_t)m);
}
