/*
 * A092349  a(n) = sigma_3(n) - sigma_2(n).
 * https://oeis.org/A092349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma_3(n) - sigma_2(n)
 * Range:     u8 n <= 6, u16 n <= 39, u32 n <= 1547, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_k.h"

seq_t a(idx_t n)
{
    /* For n >= 1, sigma_k returns the sum of the k-th powers of
       all positive divisors of n.  The difference is always
       non‑negative, so simple subtraction is safe. */
    return sigma_k(n, 3) - sigma_k(n, 2);
}
