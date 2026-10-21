/*
 * A230980  Number of primes <= n, starting at n=0.
 * https://oeis.org/A230980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes ≤ n, i.e. the prime counting function
 *            π(n)
 * Range:     u8 n <= 1618, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    return compute_pi(n);
}
