/*
 * A175264  a(n) = the middle prime among the distinct primes dividing {the
 *          n-th positive integer divisible by exactly an odd number of
 *          distinct primes}.
 * https://oeis.org/A175264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = middle prime among the distinct primes dividing the n-th
 *            positive integer whose number of distinct prime factors is odd
 * Range:     u8 n <= 113, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Find the n-th integer with an odd number of distinct prime factors. */
    idx_t target = (idx_t)n;
    idx_t count  = 0;
    uint64_t m   = 0;

    while (count < target) {
        m++;
        uint64_t temp = m;
        idx_t  distinct = 0;

        /* Count distinct prime factors of m by trial division. */
        uint64_t d = 2;
        while (d * d <= temp) {
            if (temp % d == 0) {
                distinct++;
                while (temp % d == 0)
                    temp /= d;
            }
            d++;
        }
        if (temp > 1)
            distinct++;

        if (distinct & 1)   /* odd number of distinct primes */
            count++;
    }

    /* Now factor m again to obtain the list of distinct primes. */
    uint64_t temp = m;
    uint64_t primes[64];
    idx_t psize = 0;

    uint64_t d = 2;
    while (d * d <= temp) {
        if (temp % d == 0) {
            primes[psize++] = d;
            while (temp % d == 0)
                temp /= d;
        }
        d++;
    }
    if (temp > 1)
        primes[psize++] = temp;

    /* Middle prime (psize is guaranteed odd). */
    idx_t middle = psize / 2;
    return (seq_t)primes[middle];
}
