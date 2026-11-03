/*
 * A193529  Sum of odd divisors of sopf(n).
 * https://oeis.org/A193529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of odd divisors of A008472(n) where * A008472(n) is
 *            the sum of distinct prime factors of n
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 91 terms through n = 9623 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static inline seq_t sumDistinctPrimeFactors(idx_t n)
{
    if (n < 2) return (seq_t)0;
    seq_t sum = (seq_t)0;
    uint64_t temp = (uint64_t)n;

    /* factor 2 */
    if (temp % 2ULL == 0) {
        sum += (seq_t)2;
        while (temp % 2ULL == 0) temp /= 2ULL;
    }

    /* odd factors */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            sum += (seq_t)p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) sum += (seq_t)temp;
    return sum;
}

static inline seq_t sumOddDivisors(seq_t x)
{
    if (x == (seq_t)0) return (seq_t)0;

    /* strip powers of two – only odd divisors matter */
    while ((x & (seq_t)1) == 0) x >>= 1;
    /* now x is odd, at least 1 */

    seq_t result = (seq_t)1;
    uint64_t temp = (uint64_t)x;

    /* factor odd part */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            seq_t term = (seq_t)1;
            seq_t cur   = (seq_t)1;
            while (temp % p == 0) {
                temp /= p;
                cur *= (seq_t)p;
                term += cur;
            }
            result *= term;
        }
    }
    if (temp > 1) {                 /* remaining prime */
        result *= (seq_t)(1 + temp);
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* safety guard – sequence starts at n=1 */
    seq_t sum_distinct = sumDistinctPrimeFactors(n);
    return sumOddDivisors(sum_distinct);
}
