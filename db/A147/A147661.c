/*
 * A147661  a(n) = squarefree part of n^n.
 * https://oeis.org/A147661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = squarefree part of n^n; i.e. the product of all primes p
 *            dividing n for which the exponent of p in n is odd and n is odd;
 *            otherwise 1
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Even n yields a(n) = 1 */
    if ((n & 1) == 0) {
        return (seq_t)1;
    }

    seq_t prod = (seq_t)1;
    idx_t m = n;

    /* Factor odd primes only (n is odd). */
    idx_t p = 3;
    while (p * p <= m) {
        if (m % p == 0) {
            idx_t exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }
            if (exp & 1) {
                prod *= (seq_t)p;
            }
        }
        p += 2;  /* skip even numbers */
    }
    /* Remaining prime factor (if any). */
    if (m > 1) {
        /* m is prime and comes with exponent 1 (odd). */
        prod *= (seq_t)m;
    }

    return prod;
}
