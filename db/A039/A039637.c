/*
 * A039637  Number of steps to fixed point of "n -> n/2 or (n+1)/2 until
 *          result is prime".
 * https://oeis.org/A039637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of terms in the sequence * n0 = n, * n_(i+1) =
 *            (n_i >> 1) + (n_i & 1), * stop when n_i is 1 or a prime. * a(n)
 *            is the count of terms including the starting value
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    seq_t steps = 1;                 /* count the starting term */
    while (n > 1 && !is_prime(n))
    {
        n = (n >> 1) + (n & 1);     /* next = floor((n+1)/2) */
        steps++;
    }
    return steps;
}
