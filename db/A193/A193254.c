/*
 * A193254  Sum of odd divisors of lambda(n).
 * https://oeis.org/A193254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000593(A002322(n)) – sum of odd divisors of λ(n)
 * Range:     u8 n <= 330, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_uint64, sum_odd_divisors_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_uint64.h"
#include "../utils/sum_odd_divisors_uint64.h"

/* Euclidean gcd for uint64_t */

/* LCM for uint64_t */
static inline uint64_t lcm_uint64(uint64_t a, uint64_t b) {
    return a / gcd_uint64(a, b) * b;
}

/* Compute Carmichael lambda(n) for n up to ~1e4 */
static inline uint64_t lambda_uint64(uint64_t n) {
    uint64_t lam = 1;
    uint64_t m = n;
    while (m > 1) {
        /* Smallest prime factor by trial division – fine for n <= 1e4 */
        uint64_t p = 0;
        if (m % 2 == 0) {
            p = 2;
        } else {
            for (uint64_t i = 3; i * i <= m; i += 2) {
                if (m % i == 0) {
                    p = i;
                    break;
                }
            }
            if (!p) p = m; /* m is prime */
        }

        /* Count exponent of p in m */
        uint8_t exp = 0;
        while (m % p == 0) {
            m /= p;
            exp++;
        }

        /* λ(p^exp) */
        uint64_t part = 0;
        if (p == 2) {
            if (exp == 1)
                part = 1;
            else if (exp == 2)
                part = 2;
            else
                part = ((uint64_t)1) << (exp - 2); /* 2^(exp-2) */
        } else {
            uint64_t pow = 1;
            for (uint8_t i = 0; i < exp - 1; i++) pow *= p; /* p^(exp-1) */
            part = pow * (p - 1);                           /* φ(p^exp) */
        }

        lam = lcm_uint64(lam, part);
    }
    return lam;
}

/* The required sequence function */
seq_t a(idx_t n) {
    uint64_t nn = (uint64_t)n;
    uint64_t lam = lambda_uint64(nn);
    /* Remove factors of 2 to get the odd part */
    while ((lam & 1ULL) == 0)
        lam >>= 1;
    return sum_odd_divisors_uint64(lam);
}
