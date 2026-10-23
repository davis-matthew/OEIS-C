/*
 * A136177  Number of exponents in the prime factorization of n that are
 *          coprime to n.
 * https://oeis.org/A136177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of exponents in the prime factorisation of n that
 *            are coprime to n
 * Range:     u8 n <= 631, u16 n <= 9176, u32 n <= 9176, u64 n <= 9176,
 *            u128 n <= 9176, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* n = 1 has no prime factors */
    if (n <= 1) return 0;

    idx_t value = n;                /* working copy of n   */
    seq_t sn = (seq_t)n;            /* original n as seq_t */
    seq_t count = 0;                /* result to return    */

    /* factor 2 */
    if (value % 2 == 0) {
        idx_t exp = 0;
        while (value % 2 == 0) {
            value /= 2;
            exp++;
        }
        seq_t se = (seq_t)exp;
        if (gcd(se, sn) == (seq_t)1) {
            count++;
        }
    }

    /* factor odd primes */
    for (idx_t p = 3; p * p <= value; p += 2) {
        if (value % p == 0) {
            idx_t exp = 0;
            while (value % p == 0) {
                value /= p;
                exp++;
            }
            seq_t se = (seq_t)exp;
            if (gcd(se, sn) == (seq_t)1) {
                count++;
            }
        }
    }

    /* if a prime factor > sqrt remains (>1) */
    if (value > 1) {
        /* exponent is 1 */
        if (gcd((seq_t)1, sn) == (seq_t)1) {
            count++;
        }
    }

    return count;
}
