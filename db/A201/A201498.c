/*
 * A201498  a(n) = (prime(n) - 1)*(prime(n+1) - 1)/2 + 3.
 * https://oeis.org/A201498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n) - 1)*(prime(n+1) - 1)/2 + 3
 * Range:     u8 n <= 8, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Get the n-th and (n+1)-th prime numbers */
    seq_t p1 = (seq_t)get_prime(n);
    seq_t p2 = (seq_t)get_prime(n + 1);

    /* Compute the sequence value according to the formula */
    seq_t result = ((p1 - 1) * (p2 - 1)) / 2 + 3;
    return result;
}
