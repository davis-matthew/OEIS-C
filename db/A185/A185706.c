/*
 * A185706  Characteristic function of positive numbers that are primes ending
 *          in 3.
 * https://oeis.org/A185706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 1 if n is a prime ending in 3, else 0
 * Range:     u8 n <= 9896, u16 n <= 9896, u32 n <= 9896, u64 n <= 9896,
 *            u128 n <= 9896, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    return (seq_t)((is_prime(n) && (n % 10 == 3)) ? 1 : 0);
}
