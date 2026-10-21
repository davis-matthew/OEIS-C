/*
 * A079090  Sum of distinct prime factors of (prime(n)+1)*(prime(n+1)+1)/4.
 * https://oeis.org/A079090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of distinct prime factors of
 *            ((prime(n)+1)*(prime(n+1)+1))/4
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 7352, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Retrieve the n-th and (n+1)-th primes */
    uint32_t p1 = get_prime(n);
    uint32_t p2 = get_prime(n + 1);

    /* Compute (p_n+1)*(p_{n+1}+1)/4 as a seq_t value */
    seq_t prod = (seq_t)(p1 + 1) * (seq_t)(p2 + 1);
    seq_t x = prod / 4;

    /* Factor x and sum its distinct prime divisors */
    seq_t temp = x;
    seq_t sum = 0;
    for (seq_t d = 2; d * d <= temp; ++d)
    {
        if (temp % d == 0)
        {
            sum += d;          /* add this distinct prime factor */
            while (temp % d == 0)
                temp /= d;     /* strip all powers of d */
        }
    }
    if (temp > 1)
        sum += temp;           /* remaining prime factor */

    return sum;
}
