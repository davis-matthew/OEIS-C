/*
 * A013969  a(n) = sigma_21(n), the sum of the 21st powers of the divisors of
 *          n.
 * https://oeis.org/A013969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = sigma_21(n) = ∑_{d|n} d^21
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 8, u128 n <= 68,
 *            big any n
 * Helpers:   sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_k.h"

seq_t a(idx_t n)
{
    /* The helper `sigma_k` (in utils) returns the sum of the k‑th powers of
       all divisors of n.  Using k = 21 yields OEIS A013969. */
    return sigma_k(n, 21);
}
