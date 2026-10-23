/*
 * A172407  Positive numbers n such that n+10 is a prime.
 * https://oeis.org/A172407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n+4) - 10
 * Range:     u8 n <= 52, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* OEIS offset is 1: a(1) = 1 (since 1+10 = 11 is prime) */
    if (n == 0)          /* defined behaviour for n < 1 is arbitrary */
        return 0;
    return (seq_t)get_prime(n + 4) - (seq_t)10;
}
