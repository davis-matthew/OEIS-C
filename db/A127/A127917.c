/*
 * A127917  Product of three numbers: n-th prime, previous number, and
 *          following number.
 * https://oeis.org/A127917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n) * (prime(n)^2 - 1)
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 257, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 1, so n>=1 is valid.  For n<1 we return 0. */
    if (n < 1)
        return 0;

    /* Retrieve the n-th prime.  get_prime returns a uint32_t, cast to seq_t. */
    seq_t p = (seq_t)get_prime(n);

    /* Compute p * (p^2 - 1).  All arithmetic is performed in seq_t. */
    seq_t p2 = p * p;
    seq_t res = p * (p2 - 1);

    return res;
}
