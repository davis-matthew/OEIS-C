/*
 * A036234  Number of primes <= n, if 1 is counted as a prime.
 * https://oeis.org/A036234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = compute_pi(n) + 1
 * Range:     u8 n <= 1612, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Treats 1 as a prime, so a(1)=1 and a(n)=number of primes ≤ n (excluding 1) + 1. */
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    /* compute_pi counts primes ≤ n (excluding 1).  Add 1 to count 1 as a prime. */
    return compute_pi(n) + (seq_t)1;
}
