/*
 * A065967  a(n) = n * Sum_{primes p dividing n} (1 + 1/p).
 * https://oeis.org/A065967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * Sum_{p|n} (1 + 1/p) Equivalently: a(n) = n * ω(n) +
 *            Sum_{p|n} n/p where ω(n) is the number of distinct prime factors
 *            of n
 * Range:     u8 n <= 65, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 1) return 0;          /* ω(1) = 0, no primes */
    seq_t result = 0;
    idx_t temp = n;

    /* handle factor 2 separately for speed */
    if ((temp & 1) == 0) {
        result += (seq_t)n;
        result += (seq_t)(n / 2);
        while ((temp & 1) == 0) temp >>= 1;
    }

    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            result += (seq_t)n;
            result += (seq_t)(n / p);
            while (temp % p == 0) temp /= p;
        }
    }

    /* remaining prime factor > sqrt(n) */
    if (temp > 1) {
        result += (seq_t)n;
        result += (seq_t)(n / temp);
    }

    return result;
}
