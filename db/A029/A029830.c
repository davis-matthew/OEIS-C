/*
 * A029830  Eisenstein series E_20(q) (alternate convention E_10(q)),
 *          multiplied by 174611.
 * https://oeis.org/A029830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13200 * sigma_19(n) for n > 0, a(0) = 174611, where
 *            sigma_k(n) = sum_{d|n} d^k
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 6, u128 n <= 64,
 *            big any n
 * Helpers:   pow_small_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_small_k.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)174611;
    }

    idx_t t = n;
    seq_t sigma = 1;

    /* factor 2 */
    if ((t & 1) == 0) {
        idx_t e = 0;
        while ((t & 1) == 0) {
            t >>= 1;
            ++e;
        }
        seq_t p_pow = pow_small_k((seq_t)2, 19);
        seq_t part = 1;
        seq_t term = 1;
        for (idx_t i = 0; i < e; ++i) {
            term *= p_pow;
            part += term;
        }
        sigma *= part;
    }

    /* odd prime factors */
    for (idx_t p = 3; p * p <= t; p += 2) {
        if (t % p == 0) {
            idx_t e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            seq_t p_pow = pow_small_k((seq_t)p, 19);
            seq_t part = 1;
            seq_t term = 1;
            for (idx_t i = 0; i < e; ++i) {
                term *= p_pow;
                part += term;
            }
            sigma *= part;
        }
    }

    /* remaining prime factor if any */
    if (t > 1) {
        seq_t p_pow = pow_small_k((seq_t)t, 19);
        seq_t part = 1;
        part += p_pow;   /* sigma for a prime factor with exponent 1 */
        sigma *= part;
    }

    return sigma * (seq_t)13200;
}
