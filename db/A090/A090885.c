/*
 * A090885  Sum of the squares of the exponents in the prime factorization of
 *          n.
 * https://oeis.org/A090885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum of the squares of the exponents in the prime
 *            factorization of n. This implements the additive function a(p^e)
 *            = e^2
 * Range:     u8 n <= 9577, u16 n <= 9577, u32 n <= 9577, u64 n <= 9577,
 *            u128 n <= 9577, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for clarity, types are supplied by the build */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;            /* a(1) = 0 (and a(0) irrelevant) */

    seq_t sum = 0;
    idx_t tmp = n;

    /* Handle factor 2 separately to allow increments of 2 in the loop. */
    if ((tmp & 1) == 0) {
        unsigned e = 0;
        while ((tmp & 1) == 0) {
            tmp >>= 1;
            ++e;
        }
        seq_t sq = (seq_t)e * e;
        sum += sq;
    }

    /* Loop over odd divisors up to sqrt(tmp).  Use p <= tmp/p to avoid overflow. */
    for (idx_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            unsigned e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            seq_t sq = (seq_t)e * e;
            sum += sq;
        }
    }

    /* If after the loop tmp > 1, it is a prime factor with exponent 1. */
    if (tmp > 1)
        sum += (seq_t)1;   /* 1^2 = 1 */

    return sum;
}
