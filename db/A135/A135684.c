/*
 * A135684  a(n)=11 if n is a prime number. Otherwise, a(n)=n.
 * https://oeis.org/A135684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11 if n is prime, else n
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (is_prime(n))
        return (seq_t)11;
    else
        return (seq_t)n;
}
