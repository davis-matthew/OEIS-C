/*
 * A165994  a(n) is the number of nonzero values of floor (j^2/prime(n)), over
 *          1 <= j < prime(n).
 * https://oeis.org/A165994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(prime(n) - sqrt(prime(n))) = prime(n) -
 *            floor_sqrt(prime(n)) - 1
 * Range:     u8 n <= 18, u16 n <= 1306, u32 n <= 9554, u64 n <= 9554,
 *            u128 n <= 9554, big any n
 * Helpers:   floor_sqrt, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* n is 1‑based: get_prime(1) = 2, get_prime(2) = 3, … */
    seq_t p = (seq_t)get_prime(n);

    /* floor of the square root of p */
    seq_t s = floor_sqrt(p);

    /* p is never a perfect square for prime(p) > 2, so
       floor(p - sqrt(p)) = p - ceil(sqrt(p)) = p - (s + 1) */
    return p - s - 1;
}
