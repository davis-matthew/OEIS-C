/*
 * A080339  Characteristic function of {1} union {primes}: 1 if n is 1 or a
 *          prime, else 0.
 * https://oeis.org/A080339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 1 if n == 1 or n is prime, else 0 (for n >= 1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isPrime.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    return isPrime(n) ? (seq_t)1 : (seq_t)0;
}
