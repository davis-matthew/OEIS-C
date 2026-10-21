/*
 * A229067  Sum of n-th prime and next perfect square.
 * https://oeis.org/A229067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = prime(n) + next perfect square after prime(n)
 * Range:     u8 n <= 30, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _floor_isqrt_u64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n) {
    /* The index n is 1‑based. For n < 1 the result is undefined,
       but callers will only request n >= 1. */
    if (n < 1) return 0;

    /* Get the n‑th prime (returns a 32‑bit value). */
    uint64_t p = (uint64_t)get_prime(n);

    /* Compute the smallest integer s such that s^2 > p. */
    idx_t s = _floor_isqrt_u64(p);
    /* If p itself were a perfect square (which never happens for primes),
       this would incorrectly give s^2 <= p. We therefore always use (s+1)^2. */
    seq_t next_sq = (seq_t)(s + 1) * (s + 1);

    /* Sum the prime and the next square. */
    seq_t sum = (seq_t)p + next_sq;
    return sum;
}
