/*
 * A144907  a(n) = 1 if n is prime, 2 * rad(n) if four divides n and rad(n)
 *          otherwise.
 * https://oeis.org/A144907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 if n is prime, 2*rad(n) if 4 | n, rad(n) otherwise
 * Range:     u8 n <= 257, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses helper functions is_prime(n) and rad(n) from the utils library. */

#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/rad.h"

seq_t a(idx_t n)
{
    if (is_prime(n))
        return (seq_t)1;

    seq_t r = rad(n);

    /* Four divides n iff (n & 3) == 0. */
    if ((n & 3) == 0)
        return r * (seq_t)2;

    return r;
}
