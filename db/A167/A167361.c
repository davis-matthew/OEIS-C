/*
 * A167361  Totally multiplicative sequence with a(p) = (p-3)^2 = p^2-6p+9 for
 *          prime p.
 * https://oeis.org/A167361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} ((p-3)^2)^e (for n≥1, multiplicative, a(1)=1)
 * Range:     u8 n <= 18, u16 n <= 262, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* zero if n is divisible by 3 */
    if (n % 3 == 0) {
        return (seq_t)0;
    }

    seq_t result = 1;
    idx_t m = n;

    /* trial division for factorisation */
    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            unsigned e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            /* compute ((p-3)^2)^e */
            long long diff = (long long)p - 3;
            seq_t base = (seq_t)(diff * diff);   /* (p-3)^2 */
            seq_t power = 1;
            for (unsigned i = 0; i < e; ++i) {
                power *= base;
            }
            result *= power;
        }
    }

    /* remaining prime factor if any */
    if (m > 1) {
        long long diff = (long long)m - 3;
        seq_t base = (seq_t)(diff * diff);   /* (m-3)^2, exponent is 1 */
        result *= base;
    }

    return result;
}
