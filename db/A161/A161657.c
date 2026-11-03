/*
 * A161657  a(n) = the smallest multiple of {the sum of the distinct prime
 *          divisors of n} that is >= n.
 * https://oeis.org/A161657
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(n / s) * s, where s = sum of distinct prime divisors
 *            of n
 * Range:     u8 n <= 234, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 8962 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t sum_of_distinct_prime_divisors(idx_t n)
{
    seq_t sum = 0;
    idx_t temp = n;

    if (temp % 2 == 0) {
        sum += (seq_t)2;
        while (temp % 2 == 0) temp /= 2;
    }

    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            sum += (seq_t)p;
            while (temp % p == 0) temp /= p;
        }
    }

    if (temp > 1) {
        sum += (seq_t)temp;
    }

    return sum;
}

seq_t a(idx_t n)
{
    /* For n < 2 the sequence is undefined; any value is acceptable. */
    if (n < 2) return (seq_t)n;

    seq_t s = sum_of_distinct_prime_divisors(n);
    seq_t nseq = (seq_t)n;

    /* Compute ceil(n / s) as (n + s - 1) / s */
    seq_t k = (nseq + s - 1) / s;
    return k * s;
}
