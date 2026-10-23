/*
 * A127336  Numbers that are the sum of 9 consecutive primes.
 * https://oeis.org/A127336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{i=0}^{8} prime(n+i) (sum of 9 consecutive primes)
 * Range:     u8 n <= 6, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 0; i < 9; ++i)
        sum += (seq_t)get_prime(n + i);
    return sum;
}
