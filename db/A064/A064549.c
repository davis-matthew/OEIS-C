/*
 * A064549  a(n) = n * Product_{primes p|n} p.
 * https://oeis.org/A064549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn
 * Formula:   a(n) = n * Product_{p|n} p (i.e., n multiplied by the product of
 *            the distinct prime factors of n)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* product of distinct prime factors of n (rad(n)) */
    seq_t rad = 1;
    idx_t m = n;

    /* handle factor 2 */
    if ((m & 1) == 0) {
        rad = rad * (seq_t)2;
        while ((m & 1) == 0) {
            m >>= 1;
        }
    }

    /* trial division by odd numbers */
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            rad = rad * (seq_t)p;
            while (m % p == 0) {
                m /= p;
            }
        }
    }

    /* remaining prime factor > 1 */
    if (m > 1) {
        rad = rad * (seq_t)m;
    }

    /* result = n * rad */
    return (seq_t)n * rad;
}
