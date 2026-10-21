/*
 * A076568  Greatest prime divisor of 5n+10 (sum of five successive integers).
 * https://oeis.org/A076568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = largest prime divisor of (5*n + 10)
 * Range:     u8 n <= 49, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (sum of five successive integers) */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t v = (seq_t)n * (seq_t)5 + (seq_t)10;
    seq_t maxp = 1;
    seq_t tmp = v;

    /* factor out 2 */
    while ((tmp & 1) == 0) {
        maxp = (seq_t)2;
        tmp >>= 1;
    }

    /* factor odd primes */
    for (seq_t p = (seq_t)3; p * p <= tmp; p += (seq_t)2) {
        while (tmp % p == 0) {
            maxp = p;
            tmp /= p;
        }
    }

    /* any remaining prime factor > sqrt(tmp) ? */
    if (tmp > 1)
        maxp = tmp;

    return maxp;
}
