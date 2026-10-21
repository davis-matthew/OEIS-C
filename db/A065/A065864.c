/*
 * A065864  Remainder when n is divided by the number of nonprimes not
 *          exceeding n.
 * https://oeis.org/A065864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n mod (n - pi(n))
 * Range:     u8 n <= 323, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    seq_t pi      = compute_pi(n);          /* number of primes ≤ n       */
    seq_t nonprimes = (seq_t)n - pi;       /* number of non‑primes ≤ n  */
    return (seq_t)n % nonprimes;           /* remainder of n / nonprimes */
}
