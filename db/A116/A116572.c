/*
 * A116572  a(n) = floor(prime(n)/5) for n > 2, a(1) = a(2) = 1.
 * https://oeis.org/A116572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = floor(prime(n)/5) for n > 2; a(1)=a(2)=1
 * Range:     u8 n <= 54, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n <= 2)
        return 1;
    uint32_t p = get_prime(n);
    return (seq_t)p / 5;
}
