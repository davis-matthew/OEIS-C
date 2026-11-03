/*
 * A078897  Number of times the greatest prime factor of n is a factor in all
 *          numbers <=n; a(1)=1.
 * https://oeis.org/A078897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = v_gpf(n)(n!) – the exponent of the greatest prime factor
 *            of n in the factorial n!
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 111 terms through n = 9579 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Return the largest prime divisor of m (m >= 2). */
static uint64_t largest_prime_divisor(uint64_t m)
{
    uint64_t largest = 0;
    /* factor 2 */
    while ((m & 1ULL) == 0) {
        largest = 2;
        m >>= 1;
    }
    /* odd factors */
    for (uint64_t f = 3; f <= m / f; f += 2) {
        if (m % f == 0) {
            largest = f;
            while (m % f == 0)
                m /= f;
        }
    }
    if (m > 1)            /* remaining m is prime */
        largest = m;
    return largest;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    uint64_t p = largest_prime_divisor(n);
    seq_t sum = 0;
    idx_t power = n / p;
    while (power)
    {
        sum += (seq_t)power;
        power /= p;
    }
    return sum;
}
