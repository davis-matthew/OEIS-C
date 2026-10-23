/*
 * A039641  Fixed point of "k -> k/2 or (k+1)/2 until result is prime",
 *          starting with prime(n)+1.
 * https://oeis.org/A039641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = fixed point of the map * f(k) = k/2 if k is even, *
 *            (k+1)/2 if k is odd * applied repeatedly starting from
 *            prime(n)+1 * until the result is prime. Return that prime
 * Range:     u8 n <= 99, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Start from prime(n) + 1. */
    idx_t k = (idx_t)get_prime(n) + 1;

    /* Iterate until we land on a prime. */
    while (!is_prime(k)) {
        if ((k & 1) == 0)          /* even */
            k >>= 1;               /* k / 2 */
        else
            k = (k + 1) >> 1;      /* (k + 1) / 2 */
    }

    return (seq_t)k;
}
