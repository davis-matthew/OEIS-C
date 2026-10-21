/*
 * A062354  a(n) = sigma(n)*phi(n).
 * https://oeis.org/A062354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, mult
 * Formula:   a(n) = sigma(n)*phi(n)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* compute sigma(n) * phi(n) */
    seq_t s = sigma_of(n);
    seq_t p = phi(n);
    return s * p;
}
