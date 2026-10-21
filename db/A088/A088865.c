/*
 * A088865  (Sum of distinct prime factors)^(sum of prime exponents).
 * https://oeis.org/A088865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A008472(n)^A001222(n)
 * Range:     u8 n <= 19, u16 n <= 119, u32 n <= 1311, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/smallest_prime_factor.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* a(1) = 1 by convention (empty factorization) */
    if (n == 1) return (seq_t)1;

    uint64_t t = (uint64_t)n;
    uint64_t sum_primes = 0;
    uint64_t sum_exp    = 0;

    while (t > 1) {
        uint64_t p = smallest_prime_factor(t);
        uint64_t cnt = 0;
        while (t % p == 0) {
            t /= p;
            ++cnt;
        }
        sum_primes += p;
        sum_exp    += cnt;
    }

    return big_pow((seq_t)sum_primes, (seq_t)sum_exp);
}
