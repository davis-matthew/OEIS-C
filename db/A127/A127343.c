/*
 * A127343  Product of 11 consecutive primes.
 * https://oeis.org/A127343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product of 11 consecutive primes starting at prime(n)
 * Range:     u8 none, u16 none, u32 none, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    seq_t prod = 1;
    for (idx_t i = 0; i < 11; ++i)
        prod *= (seq_t) get_prime(n + i);
    return prod;
}
