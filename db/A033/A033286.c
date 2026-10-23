/*
 * A033286  a(n) = n * prime(n).
 * https://oeis.org/A033286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * prime(n)
 * Range:     u8 n <= 9, u16 n <= 109, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)   /* not defined for offset 0, but defensive */
        return 0;
    seq_t p = (seq_t)get_prime(n);   /* get_prime returns the nth prime */
    return (seq_t)n * p;             /* multiply n by prime(n) */
}
