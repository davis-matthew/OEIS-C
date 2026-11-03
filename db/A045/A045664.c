/*
 * A045664  Number of 2n-bead balanced binary strings of fundamental period
 *          2n, rotationally equivalent to reversed complement.
 * https://oeis.org/A045664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n*Sum_{d|n} mu(n/d)*2^(d-1) (a(0)=1)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 979 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/pow2.h"

/* Möbius function for the divisor in the loop */
static inline int mu_of(idx_t d) {
    int n = (int)d;
    int sign = 1;
    int i = 2;
    while ((int64_t)i * i <= n) {
        if (n % i == 0) {
            int cnt = 0;
            while (n % i == 0) {
                n /= i;
                ++cnt;
                if (cnt > 1) return 0;        /* square factor */
            }
            sign = -sign;
        }
        ++i;
    }
    if (n > 1) sign = -sign;
    return sign;
}

seq_t a(idx_t n) {
    /* Base case */
    if (n == 0) return (seq_t)1;

    seq_t sum = 0;
    idx_t sqrt_n = (idx_t)0;
    /* Calculate integer square root of n to iterate divisors */
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            /* First divisor d1 = i, second d2 = n/i */
            idx_t d1 = i;
            idx_t d2 = n / i;

            /* mu(n/d1) = mu(d2) */
            int mu1 = mu_of(d2);
            seq_t term1 = pow2(d1 - 1);
            if (mu1 > 0) {
                sum += term1;
            } else if (mu1 < 0) {
                sum -= term1;
            }

            if (d1 != d2) {
                /* mu(n/d2) = mu(d1) */
                int mu2 = mu_of(d1);
                seq_t term2 = pow2(d2 - 1);
                if (mu2 > 0) {
                    sum += term2;
                } else if (mu2 < 0) {
                    sum -= term2;
                }
            }
        }
    }

    /* a(n) = 2*n*sum */
    seq_t two_n = (seq_t)2 * (seq_t)n;
    return sum * two_n;
}
