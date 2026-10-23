/*
 * A181569  Greatest common divisor of n! and n+1.
 * https://oeis.org/A181569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = gcd(n!, n+1)
 * Range:     u8 n <= 254, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for offset 1, but safe */

    /* factorisation of n+1 */
    uint64_t m = (uint64_t)(n + 1);      /* n+1 fits in 64 bits for n up to 10000 */
    seq_t result = (seq_t)1;
    uint64_t p = 2;

    /* trial division by primes up to sqrt(m) */
    while (p * p <= m) {
        if (m % p == 0) {
            /* count exponent of p in n+1 */
            unsigned int e_n1 = 0;
            while (m % p == 0) {
                m /= p;
                ++e_n1;
            }

            /* compute exponent of p in n! (Legendre) */
            unsigned int e_fact = 0;
            idx_t tmp = n;
            while (tmp) {
                tmp /= p;
                e_fact += (unsigned int)tmp;
            }

            /* gcd exponent is the minimum */
            unsigned int e_gcd = e_n1 < e_fact ? e_n1 : e_fact;

            /* multiply result by p^e_gcd */
            seq_t pp = (seq_t)p;
            for (unsigned int i = 0; i < e_gcd; ++i)
                result *= pp;
        }
        ++p;   /* next integer; only small range needed, primes are accepted */
    }

    /* if remaining m > 1, it is a prime factor > sqrt(original m) */
    if (m > 1) {
        uint64_t prime = m;
        unsigned int e_n1 = 1;  /* exponent in n+1 is 1 */
        unsigned int e_fact = 0;
        idx_t tmp = n;
        while (tmp) {
            tmp /= prime;
            e_fact += (unsigned int)tmp;
        }
        unsigned int e_gcd = e_n1 < e_fact ? e_n1 : e_fact;
        seq_t pp = (seq_t)prime;
        for (unsigned int i = 0; i < e_gcd; ++i)
            result *= pp;
    }

    return result;
}
