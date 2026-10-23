/*
 * A185709  Characteristic function of positive numbers that are primes ending
 *          in 9.
 * https://oeis.org/A185709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 1 if n is a prime ending with 9, else 0
 * Range:     u8 n <= 9670, u16 n <= 9670, u32 n <= 9670, u64 n <= 9670,
 *            u128 n <= 9670, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    return (seq_t)(is_prime(n) && (n % 10ULL == 9ULL));
}
