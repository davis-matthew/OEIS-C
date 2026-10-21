/*
 * A053166  Smallest positive integer for which n divides a(n)^4.
 * https://oeis.org/A053166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product over prime powers p^e || n of p^{ceil(e/4)}
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)n;

    uint64_t tmp = (uint64_t)n;
    seq_t res = 1;

    /* factor powers of 2 */
    int c = 0;
    while ((tmp & 1ULL) == 0) {
        tmp >>= 1;
        c++;
    }
    if (c > 0) {
        int exp = (c + 3) / 4;          /* ceil(c/4) */
        for (int i = 0; i < exp; ++i)
            res *= 2;
    }

    /* factor odd primes */
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            c = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++c;
            }
            int exp = (c + 3) / 4;
            for (int i = 0; i < exp; ++i)
                res *= (seq_t)p;
        }
    }

    /* remaining prime factor */
    if (tmp > 1) {
        /* exponent is 1, ceil(1/4) = 1 */
        res *= (seq_t)tmp;
    }

    return res;
}
