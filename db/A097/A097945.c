/*
 * A097945  a(n) = mu(n)*phi(n) where mu(n) is the Mobius function (A008683)
 *          and phi(n) is the Euler totient function (A000010).
 * https://oeis.org/A097945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = mu(n)*phi(n)
 * Range:     i8 n <= 129, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   mobius, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mobius.h"
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    seq_t mu = mobius(n);
    if (mu == 0)
        return 0;
    seq_t phi_val = phi(n);
    return mu * phi_val;
}
