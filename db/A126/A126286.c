/*
 * A126286  a(1) = 2, a(n) = n * LeastPrimeFactor(n+1) / LeastPrimeFactor(n).
 * https://oeis.org/A126286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n * LeastPrimeFactor(n+1) / LeastPrimeFactor(n)
 * Range:     u8 n <= 15, u16 n <= 365, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return the least prime factor of n (n <= UINT64_MAX).
   For n <= 1 the value is returned unchanged. */
static uint64_t least_prime_factor(uint64_t n)
{
    if (n <= 1) return n;
    if ((n & 1) == 0) return 2;
    for (uint64_t d = 3; d <= n / d; d += 2)
        if (n % d == 0) return d;
    return n;           /* n is prime */
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;

    uint64_t spf_n  = least_prime_factor((uint64_t)n);
    uint64_t spf_n1 = least_prime_factor((uint64_t)(n + 1));

    seq_t result = (seq_t)n * (seq_t)spf_n1;
    result /= (seq_t)spf_n;
    return result;
}
