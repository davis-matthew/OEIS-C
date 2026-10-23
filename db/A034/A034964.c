/*
 * A034964  Sums of five consecutive primes.
 * https://oeis.org/A034964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)+prime(n+1)+prime(n+2)+prime(n+3)+prime(n+4)
 * Range:     u8 n <= 13, u16 n <= 1557, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 0; i < 5; ++i)
        sum += (seq_t)get_prime(n + i);
    return sum;
}
