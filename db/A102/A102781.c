/*
 * A102781  Number of positive even numbers less than the n-th prime.
 * https://oeis.org/A102781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (prime(n)-1)/2
 * Range:     u8 n <= 54, u16 n <= 9572, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* get_prime returns the n-th prime as a seq_t. */
    seq_t p = get_prime(n);
    /* Number of positive even numbers less than p is (p-1)/2. */
    return (p - 1) / 2;
}
