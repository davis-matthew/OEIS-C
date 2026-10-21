/*
 * A228620  a(n) = n - phi(n) + mu(n).
 * https://oeis.org/A228620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - phi(n) + mu(n)
 * Range:     u8 n <= 294, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   mobius, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"
#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    return tn - phi(n) + mobius(n);
}
