/*
 * A065994  a(n) = prime(prime(n) - n).
 * https://oeis.org/A065994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(prime(n) - n)
 * Range:     u8 n <= 21, u16 n <= 949, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* compute the inner index: prime(n) - n */
    idx_t inner = get_prime(n) - n;
    /* compute the outer prime using that index */
    idx_t outer = get_prime(inner);
    return (seq_t)outer;
}
