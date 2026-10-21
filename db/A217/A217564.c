/*
 * A217564  Number of primes between prime(n)/2 and prime(n+1)/2.
 * https://oeis.org/A217564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pi(prime(n+1)/2) - pi(prime(n)/2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_pi, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    /* n is 1‑based */
    uint32_t p1 = get_prime(n);        /* prime(n)   */
    uint32_t p2 = get_prime(n + 1);    /* prime(n+1) */

    idx_t half1 = (idx_t)(p1 / 2);
    idx_t half2 = (idx_t)(p2 / 2);

    seq_t pi1 = compute_pi(half1);
    seq_t pi2 = compute_pi(half2);

    return pi2 - pi1;
}
