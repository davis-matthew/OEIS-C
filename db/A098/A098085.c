/*
 * A098085  Primes P(n)+P(n+1)+b(n) = least prime >= P(n)+P(n+1), P(i)=i-th
 *          prime, b(n) given in A098084.
 * https://oeis.org/A098085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   get_prime, is_prime_small, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the least prime greater than or equal to P(n)+P(n+1),
   where P(i) is the i‑th prime.  This uses the helper
   functions get_prime, is_prime_small and next_prime. */
#include "../utils/get_prime.h"
#include "../utils/is_prime_small.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* get the nth and (n+1)th primes */
    uint64_t p1 = get_prime(n);
    uint64_t p2 = get_prime(n + 1);

    /* sum of the two primes */
    uint64_t sum = p1 + p2;

    /* if the sum itself is prime, it is the answer */
    if (is_prime_small((seq_t)sum))
        return (seq_t)sum;

    /* otherwise find the next prime after the sum */
    uint64_t nextp = next_prime(sum);
    return (seq_t)nextp;
}
