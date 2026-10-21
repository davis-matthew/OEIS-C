/*
 * A081810  If n = p_1^e_1 * ... * p_k^e_k, p_1 < ... < p_k primes, then a(n)
 *          = Max{ p_i*e_i }.
 * https://oeis.org/A081810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Max{p_i*e_i} over the prime factorization n = ∏p_i^e_i
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;

    idx_t m = n;
    seq_t max_val = 0;

    /* trial division up to √m, using m/p to avoid overflow          */
    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            idx_t exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }
            seq_t val = (seq_t)p * (seq_t)exp;
            if (val > max_val) max_val = val;
        }
    }

    /* any remaining prime factor > √original n                          */
    if (m > 1) {
        seq_t val = (seq_t)m * (seq_t)1;
        if (val > max_val) max_val = val;
    }

    return max_val;
}
