/*
 * A009223  a(n) = gcd(sigma(n), phi(n)).
 * https://oeis.org/A009223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(sigma(n), phi(n))
 * Range:     u8 n <= 18, u16 n <= 5871, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* multiply base^exp (exp >= 0) */
static seq_t pow_seq(seq_t base, int exp)
{
    seq_t r = 1;
    for (int i = 0; i < exp; ++i)
        r *= base;
    return r;
}

seq_t a(idx_t n)
{
    /* n==0 is outside the official offset; return 0 for safety. */
    if (n == 0)
        return 0;

    uint64_t m = (uint64_t)n;
    seq_t sigma_val = 1;
    seq_t phi_val   = (seq_t)n;

    while (m > 1) {
        uint64_t p = 0;
        int exp = 0;

        /* factor 2 separately */
        if (m % 2 == 0) {
            p = 2;
            do {
                m /= 2;
                ++exp;
            } while (m % 2 == 0);
        } else {
            /* try odd divisors up to sqrt(m) */
            for (uint64_t d = 3; d * d <= m; d += 2) {
                if (m % d == 0) {
                    p = d;
                    do {
                        m /= d;
                        ++exp;
                    } while (m % d == 0);
                    break;
                }
            }
            /* remaining m is a prime > sqrt(original m) */
            if (p == 0) {
                p = m;
                exp = 1;
                m = 1;
            }
        }

        seq_t p_seq = (seq_t)p;
        seq_t sigma_factor = (pow_seq(p_seq, exp + 1) - 1) / (p_seq - 1);
        sigma_val *= sigma_factor;

        phi_val /= p_seq;
        phi_val *= (p_seq - 1);
    }

    return gcd(sigma_val, phi_val);
}
