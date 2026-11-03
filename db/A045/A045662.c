/*
 * A045662  Number of 2n-bead balanced binary strings of fundamental period
 *          2n, rotationally equivalent to reverse.
 * https://oeis.org/A045662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n*Sum_{d|n} mu(n/d) * binomial(2*floor(d/2),
 *            floor(d/2))
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 123,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 1000 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"

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

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t pos = 0;
    seq_t neg = 0;

    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            /* divisor d */
            int mu1 = mu_of(n / d);
            seq_t comb1 = binomial(2 * (d >> 1), d >> 1);
            if (mu1 == 1)
                pos += comb1;
            else if (mu1 == -1)
                neg += comb1;

            /* paired divisor n/d */
            idx_t d2 = n / d;
            if (d2 != d) {
                int mu2 = mu_of(d);
                seq_t comb2 = binomial(2 * (d2 >> 1), d2 >> 1);
                if (mu2 == 1)
                    pos += comb2;
                else if (mu2 == -1)
                    neg += comb2;
            }
        }
    }

    seq_t diff = pos;
    diff -= neg;
    return (seq_t)2 * n * diff;
}
