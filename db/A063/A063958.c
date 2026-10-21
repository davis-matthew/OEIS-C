/*
 * A063958  Sum of the non-unitary prime divisors of n: sum of those prime
 *          divisors for which the exponent in the prime factorization exceeds
 *          1.
 * https://oeis.org/A063958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{p^2 | n} p (sum of primes whose exponent in the
 *            factorisation of n is at least 2)
 * Range:     u8 n <= 9738, u16 n <= 9738, u32 n <= 9738, u64 n <= 9738,
 *            u128 n <= 9738, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    uint64_t x = n;          /* n is guaranteed to fit in uint64_t   */
    uint64_t cnt = 0;

    /* handle factor 2 */
    while ((x & 1ULL) == 0ULL) {
        cnt++;
        x >>= 1;
    }
    if (cnt >= 2) {
        sum += (seq_t)2;
    }

    /* handle odd factors */
    for (uint64_t p = 3; p * p <= x; p += 2) {
        cnt = 0;
        while (x % p == 0) {
            cnt++;
            x /= p;
        }
        if (cnt >= 2) {
            sum += (seq_t)p;
        }
    }

    /* remaining x > 1 is a prime with exponent 1 – ignore */
    return sum;
}
