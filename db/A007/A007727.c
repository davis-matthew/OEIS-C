/*
 * A007727  Number of 2n-bead black-white strings with n black beads and
 *          fundamental period 2n.
 * https://oeis.org/A007727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} μ(n/d) * C(2d, d) where μ is the Möbius
 *            function
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

static int moebius(idx_t n) {
    if (n == 1) return 1;
    
    int sign = 1;
    idx_t temp = n;
    
    for (idx_t p = 2; p * p <= temp; p++) {
        if (temp % p == 0) {
            if (temp % (p * p) == 0) {
                return 0;
            }
            sign = -sign;
            temp /= p;
        }
    }
    
    if (temp > 1) {
        sign = -sign;
    }
    
    return sign;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    
    seq_t result = 0;
    
    for (idx_t d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            int mu = moebius(n / d);
            seq_t binom_coeff = binom(2 * d, d);
            if (mu > 0) {
                result += binom_coeff;
            } else if (mu < 0) {
                result -= binom_coeff;
            }
            
            if (d != n / d) {
                idx_t d2 = n / d;
                mu = moebius(n / d2);
                binom_coeff = binom(2 * d2, d2);
                if (mu > 0) {
                    result += binom_coeff;
                } else if (mu < 0) {
                    result -= binom_coeff;
                }
            }
        }
    }
    
    return result;
}
