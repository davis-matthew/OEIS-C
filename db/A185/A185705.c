/*
 * A185705  Characteristic function of positive numbers that are primes ending
 *          in 1.
 * https://oeis.org/A185705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (is_prime(n) && n%10 == 1) ? 1 : 0
 * Range:     u8 n <= 9900, u16 n <= 9900, u32 n <= 9900, u64 n <= 9900,
 *            u128 n <= 9900, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 11) return (seq_t)0;
    if (n % 10u != 1u) return (seq_t)0;
    return is_prime(n) ? (seq_t)1 : (seq_t)0;
}
