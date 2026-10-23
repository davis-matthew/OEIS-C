/*
 * A042939  Absolute values between digits of primes.
 * https://oeis.org/A042939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = |(first digit of the nth prime) - (sum of the remaining
 *            digits)|
 * Range:     u8 n <= 54, u16 n <= 6882, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Obtain the nth prime (1‑based) */
    uint32_t p = get_prime(n);
    seq_t prime = (seq_t)p;

    /* Find the first digit of the prime */
    seq_t tmp = prime;
    seq_t first;
    while (tmp >= (seq_t)10)
    {
        tmp /= (seq_t)10;
    }
    first = tmp;

    /* Sum all digits of the prime */
    seq_t sum_digits = 0;
    seq_t tmp2 = prime;
    while (tmp2 > 0)
    {
        sum_digits += tmp2 % (seq_t)10;
        tmp2 /= (seq_t)10;
    }

    /* Sum of remaining digits */
    seq_t sum_rest = sum_digits - first;

    /* Absolute difference between first digit and sum of the rest */
    seq_t diff;
    if (first >= sum_rest)
        diff = first - sum_rest;
    else
        diff = sum_rest - first;

    return diff;
}
