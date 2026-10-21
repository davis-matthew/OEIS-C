/*
 * A201059  Denominator of binomial(2n,n)/(2n).
 * https://oeis.org/A201059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of binomial(2 n, n)/(2 n)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1; /* undefined for n=0, but return 1 for safety */

    uint64_t two_n = 2 * (uint64_t)n;
    uint64_t temp   = two_n;
    seq_t   result  = 1;

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            /* exponent of p in 2n */
            uint64_t exp_m = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp_m;
            }

            /* exponent of p in binomial(2n, n) = v_p((2n)!) - 2*v_p(n!) */
            uint64_t v = 0, k = two_n;
            while (k) { k /= p; v += k; }          /* v_p((2n)!) */
            uint64_t v2 = 0, k2 = n;
            while (k2) { k2 /= p; v2 += k2; }     /* v_p(n!) */
            uint64_t exp_bin = v - 2 * v2;

            if (exp_bin < exp_m) {
                uint64_t exp_den = exp_m - exp_bin;
                seq_t  p_pow     = big_pow((seq_t)p, (seq_t)exp_den);
                result *= p_pow;
            }
        }
    }

    if (temp > 1) { /* remaining prime factor */
        uint64_t p     = temp;
        uint64_t exp_m = 1;

        uint64_t v = 0, k = two_n;
        while (k) { k /= p; v += k; }
        uint64_t v2 = 0, k2 = n;
        while (k2) { k2 /= p; v2 += k2; }
        uint64_t exp_bin = v - 2 * v2;

        if (exp_bin < exp_m) {
            uint64_t exp_den = exp_m - exp_bin;
            seq_t  p_pow     = big_pow((seq_t)p, (seq_t)exp_den);
            result *= p_pow;
        }
    }
    return result;
}
