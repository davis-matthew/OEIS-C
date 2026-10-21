/*
 * A070777  a(1) = 1; a(n) = (largest prime factor of n) - 1.
 * https://oeis.org/A070777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (largest prime factor of n) – 1, with a(1)=1
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation factors n on the fly using trial division. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;           /* a(1)=1 by definition */

    idx_t tmp = n;
    idx_t lpf = 1;          /* largest prime factor found so far */

    /* factor out a 2 if present */
    if ((tmp & 1) == 0) {
        lpf = 2;
        while ((tmp & 1) == 0)
            tmp >>= 1;
    }

    /* test odd divisors up to sqrt(tmp) */
    for (idx_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            lpf = p;
            while (tmp % p == 0)
                tmp /= p;
        }
    }

    /* if anything remains it is prime */
    if (tmp > 1)
        lpf = tmp;

    return (seq_t)lpf - 1;
}
