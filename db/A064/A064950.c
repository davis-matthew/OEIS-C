/*
 * A064950  a(n) = Sum_{i|n, j|n} lcm(i,j).
 * https://oeis.org/A064950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn, easy
 * Formula:   a(n) = Sum_{d|n} d * tau(d^2)
 * Range:     u8 n <= 11, u16 n <= 359, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Derived from a(p^e) = (p^(e+2) - 3*p^(e+1) + p + 1
   + 2*p^(e+2)*e - 2*p^(e+1)*e) / (p-1)^2, multiplicative over n. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_mul.h"

/* Small helper: compute p^e for uint64_t p and integer e (e >= 0). */
static uint64_t pow_u64(uint64_t p, int e)
{
    uint64_t r = 1;
    while (e > 0) {
        r *= p;
        --e;
    }
    return r;
}

/* Compute a prime power contribution a(p^e) using 64‑bit arithmetic
   and cast the final integer to seq_t. */
static seq_t prime_power_term(uint64_t p, int e)
{
    uint64_t pe1 = pow_u64(p, e + 1);   /* p^(e+1) */
    uint64_t pe2 = pe1 * p;             /* p^(e+2) */

    /* Use signed 64‑bit for intermediate to avoid unsigned underflow. */
    int64_t num = (int64_t)pe2
               - 3LL * (int64_t)pe1
               + (int64_t)p
               + 1
               + 2LL * (int64_t)pe2 * e
               - 2LL * (int64_t)pe1 * e;

    uint64_t denom = (uint64_t)(p - 1) * (p - 1);
    uint64_t term = (uint64_t)(num / denom);

    return (seq_t)term;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    uint64_t m = (uint64_t)n;
    seq_t result = (seq_t)1;

    /* Trial division factorisation. */
    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            seq_t term = prime_power_term(p, exp);
            result = seq_mul(result, term);
        }
    }
    if (m > 1) {                 /* Remaining prime factor with exp = 1 */
        seq_t term = prime_power_term(m, 1);
        result = seq_mul(result, term);
    }
    return result;
}
