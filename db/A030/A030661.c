/*
 * A030661  Product of next 2 primes after n.
 * https://oeis.org/A030661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of the two smallest primes greater than n
 * Range:     u8 n <= 12, u16 n <= 250, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* next_prime expects a uint64_t argument, n may have the same type, but cast to be safe */
    uint64_t p1 = next_prime((uint64_t)n);
    uint64_t p2 = next_prime(p1);
    return (seq_t)p1 * (seq_t)p2;
}
