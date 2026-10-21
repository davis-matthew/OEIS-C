/*
 * A075653  a(n) = n + sopf(n), where sopf is the sum of the distinct prime
 *          factors of n (A008472).
 * https://oeis.org/A075653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n + A008472(n)
 * Range:     u8 n <= 130, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* A008472: sum of distinct prime factors of n */
    uint64_t temp = n;
    uint64_t sum = 0;

    if ((temp & 1) == 0) {          /* handle factor 2 */
        sum += 2;
        while ((temp & 1) == 0) temp >>= 1;
    }

    for (uint64_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            sum += p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) sum += temp;      /* remaining prime factor */

    return (seq_t)n + (seq_t)sum;
}
