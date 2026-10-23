/*
 * A023569  Greatest prime divisor of prime(n) - 3.
 * https://oeis.org/A023569
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest_prime_divisor(prime(n) - 3)
 * Range:     u8 n <= 99, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, largest_prime_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/largest_prime_divisor.h"

seq_t a(idx_t n) {
    /* Sequence is defined for n ≥ 3; for other n we return 0.  */
    if (n < 3)
        return 0;

    /* Get the n-th prime.  get_prime returns a 32‑bit prime.  */
    uint32_t p = get_prime(n);

    /* Compute the greatest prime divisor of (prime(n) - 3).  */
    uint64_t m = (uint64_t)p - 3ULL;
    uint64_t gpf = largest_prime_divisor(m);

    /* Convert the result to seq_t (works for both fixed‑size
       integer types and arbitrary‑precision classes).  */
    return (seq_t)gpf;
}
