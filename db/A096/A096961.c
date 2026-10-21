/*
 * A096961  a(n) = Sum_{0<d|n, n/d odd} d^7.
 * https://oeis.org/A096961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{0<d|n, n/d odd} d^7 Multiplicative definition: •
 *            a(1) = 1 • a(2^e) = 2^(7e) • for an odd prime p, a(p^e) = 1 +
 *            p^7 + p^14 + … + p^(7e)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 23, u64 n <= 565,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for 0, but harmless */

    uint64_t nn = (uint64_t)n;

    /* Separate out the power of 2 */
    uint64_t m = nn;
    unsigned e2 = 0;
    while ((m & 1ULL) == 0) {
        m >>= 1;
        e2++;
    }

    /* Compute 2^(7*e2) */
    seq_t result = 1;
    for (unsigned i = 0; i < e2 * 7; ++i)
        result *= (seq_t)2;

    /* Factor the odd part m */
    uint64_t p = 3;
    while (p * p <= m) {
        if (m % p == 0) {
            unsigned exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }

            /* Compute 1 + p^7 + p^14 + … + p^(7*exp) */
            seq_t sum   = 1;
            seq_t term  = 1;
            seq_t p_pow7 = 1;
            for (int i = 0; i < 7; ++i)
                p_pow7 *= (seq_t)p;

            for (unsigned i = 1; i <= exp; ++i) {
                term *= p_pow7;   /* term = p^(7*i) */
                sum  += term;     /* add the new term */
            }
            result *= sum;
        }
        p += 2;
    }

    if (m > 1) {               /* final prime factor with exponent 1 */
        uint64_t prime = m;

        seq_t sum  = 1;
        seq_t term = 1;
        seq_t p_pow7 = 1;
        for (int i = 0; i < 7; ++i)
            p_pow7 *= (seq_t)prime;

        term *= p_pow7;               /* i = 1 */
        sum  += term;
        result *= sum;
    }

    return result;
}
