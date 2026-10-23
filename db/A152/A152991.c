/*
 * A152991  a(n) = sigma(n) - pi(n).
 * https://oeis.org/A152991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sigma(n) - pi(n)
 * Range:     u8 n <= 119, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   compute_pi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    /* Ensure we compute for n >= 1; behaviour for n < 1 is irrelevant to OEIS */
    return sigma_of(n) - compute_pi(n);
}
