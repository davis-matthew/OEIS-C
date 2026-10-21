/*
 * A059376  Jordan function J_3(n).
 * https://oeis.org/A059376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A059376 – Jordan function J_3(n)
 *
 *  Formula:  a(n) = Σ_{d|n} d³ μ(n/d)
 *           = n³ ∏_{p|n} (1 – 1/p³)
 *  For a prime power p^e: a(p^e) = p^(3e) – p^(3e – 3)
 *
 *  The implementation factors n by trial division, computes each
 *  prime‑power contribution, and multiplies them together.
 *  It works for all n ≥ 1 and uses only integer arithmetic.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;          /* a(1) = 1 */

    idx_t m = n;
    seq_t result = 1;

    /* handle factor 2 */
    int exp = 0;
    while ((m & 1ULL) == 0) {
        ++exp;
        m >>= 1;
    }
    if (exp) {
        seq_t p3 = (seq_t)2 * 2 * 2;            /* 2³ */
        seq_t pow = 1;
        for (int i = 0; i < exp; ++i)
            pow *= p3;                          /* (2³)^exp = 2^(3*exp) */
        seq_t pow_minus = pow / p3;             /* 2^(3*exp – 3) */
        result *= (pow - pow_minus);
    }

    /* odd prime factors */
    idx_t i = 3;
    while (i <= m / i) {
        if (m % i == 0) {
            exp = 0;
            while (m % i == 0) {
                ++exp;
                m /= i;
            }
            seq_t p3 = (seq_t)i * i * i;       /* i³ */
            seq_t pow = 1;
            for (int k = 0; k < exp; ++k)
                pow *= p3;                      /* (i³)^exp = i^(3*exp) */
            seq_t pow_minus = pow / p3;         /* i^(3*exp – 3) */
            result *= (pow - pow_minus);
        }
        i += 2;
    }

    /* a remaining prime factor > sqrt(original n) */
    if (m > 1) {
        seq_t p3 = (seq_t)m * m * m;            /* m³ */
        seq_t contrib = p3 - 1;                 /* e = 1 : m³ – m⁰ */
        result *= contrib;
    }

    return result;
}
