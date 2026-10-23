/*
 * A185708  Characteristic function of positive numbers that are primes ending
 *          in 7.
 * https://oeis.org/A185708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 1 if n is prime and ends with digit 7, otherwise 0
 * Range:     u8 n <= 9888, u16 n <= 9888, u32 n <= 9888, u64 n <= 9888,
 *            u128 n <= 9888, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n % 10 == 7 && is_prime(n))
        return (seq_t)1;
    return (seq_t)0;
}
