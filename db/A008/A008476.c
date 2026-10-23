/*
 * A008476  If n = Product (p_j^k_j) then a(n) = Sum (k_j^p_j).
 * https://oeis.org/A008476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum over prime powers p^k in the factorization of n of
 *            k^p (OEIS A008476)
 * Range:     u8 n <= 120, u16 n <= 288, u32 n <= 1368, u64 n <= 4488,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;                 /* a(1) = 0 */

    uint64_t m = (uint64_t)n;
    seq_t sum = (seq_t)0;

    /* Trial division to find prime factors */
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }

            /* compute exp^p */
            seq_t term = (seq_t)1;
            for (uint64_t i = 0; i < p; ++i)
                term *= (seq_t)exp;

            sum += term;
        }
    }

    /* If a prime factor larger than sqrt(m) remains */
    if (m > 1) {               /* m is prime with exponent 1 */
        uint64_t p = m;
        /* term = 1^p = 1 */
        sum += (seq_t)1;
    }

    return sum;
}
