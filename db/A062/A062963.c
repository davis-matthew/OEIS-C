/*
 * A062963  a(n) = mu(n) * H(n) where H(n) is A023022.
 * https://oeis.org/A062963
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = mu(n) * (phi(n)+1)/2
 * Range:     i8 n <= 261, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   mobius, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the OEIS sequence A062963: a(n) = mu(n) * H(n) where
 * H(n) = (phi(n)+1)/2.  The function is defined for all n >= 2.
 */

#include <stdint.h>
#include "../utils/mobius.h"
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* mu(1) would be 1, but the OEIS offset starts at 2; 
       for completeness we return 0 for n < 2. */
    if (n < 2)
        return (seq_t)0;

    seq_t mu = mobius(n);
    if (mu == 0)
        return (seq_t)0;          /* not square‑free, product is 0 */

    seq_t phi_val = phi(n);
    seq_t H = (phi_val + (seq_t)1) / (seq_t)2; /* ceil(phi/2) */
    return mu * H;
}
