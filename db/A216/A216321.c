/*
 * A216321  phi(delta(n)), n >= 1, with phi = A000010 (Euler's totient) and
 *          delta = A055034 (degree of minimal polynomials with coefficients
 *          given in A187360).
 * https://oeis.org/A216321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(delta(n)) with delta(1) = 1, delta(n) = phi(2*n)/2
 *            for n >= 2 and phi = Euler's totient (A000010)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/calc_phi.h"

seq_t
a(idx_t n)
{
    /* delta(1) = 1, phi(1) = 1 */
    if (n == 1)
        return calc_phi((seq_t)1);

    /* compute delta(n) = phi(2*n)/2 */
    seq_t two_n   = (seq_t)n * 2;   /* 2*n fits in seq_t for all n used in tests */
    seq_t phi2n   = calc_phi(two_n);
    seq_t delta   = phi2n / 2;      /* delta(n) is an integer */

    /* return phi(delta(n)) */
    return calc_phi(delta);
}
