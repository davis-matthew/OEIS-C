/*
 * A076559  Greatest prime divisor of n-th interprime: (prime(n) +
 *          prime(n+1))/2.
 * https://oeis.org/A076559
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = greatest prime divisor of the n-th interprime (interprime
 *            = floor((prime(n)+prime(n+1))/2))
 * Range:     u8 n <= 102, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, largest_prime_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/largest_prime_divisor.h"

seq_t a(idx_t n)
{
    if (n < 2)   /* offset starts at 2 – behavior below is undefined */
        return 0;

    /* compute the n-th and (n+1)-st primes */
    uint64_t p1 = (uint64_t)get_prime(n);
    uint64_t p2 = (uint64_t)get_prime(n + 1);

    /* interprime = floor((p1 + p2) / 2) */
    uint64_t inter = (p1 + p2) / 2;

    /* greatest prime divisor of the interprime */
    uint64_t gpf_val = largest_prime_divisor(inter);

    return (seq_t)gpf_val;
}
