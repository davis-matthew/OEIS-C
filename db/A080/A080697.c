/*
 * A080697  a(n) = n * prime(prime(n)).
 * https://oeis.org/A080697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * prime(prime(n)) (offset = 1)
 * Range:     u8 n <= 6, u16 n <= 47, u32 n <= 5920, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* undefined for n < 1 */

    /* nth prime */
    uint32_t p1 = get_prime((idx_t)n);

    /* p1-th prime */
    uint32_t p2 = get_prime((idx_t)p1);

    return (seq_t)n * (seq_t)p2;
}
