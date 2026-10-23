/*
 * A174223  Add to 2n+1 the next three primes > 2n+1.
 * https://oeis.org/A174223
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2n+1) + next_prime(2n+1) + next_prime(next_prime(2n+1))
 *            + next_prime(next_prime(next_prime(2n+1)))
 * Range:     u8 n <= 28, u16 n <= 8183, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n) {
    seq_t base = (seq_t)n * 2 + 1;          /* 2n+1 */
    uint64_t p1 = next_prime((uint64_t)base);
    uint64_t p2 = next_prime(p1);
    uint64_t p3 = next_prime(p2);
    seq_t result = base + (seq_t)p1 + (seq_t)p2 + (seq_t)p3;
    return result;
}
