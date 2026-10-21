/*
 * A224339  Absolute difference between sum of odd divisors of n^2 and sum of
 *          even divisors of n^2.
 * https://oeis.org/A224339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = (-1)^n * Σ_{d|n²} (-1)^d · d Equivalent multiplicative
 *            form: a(1) = 1 a(2^e) = 2^(2e+1) – 3 a(p^e) = (p^(2e+1) – 1)/(p
 *            – 1) for odd prime p
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    seq_t result = 1;
    idx_t m = n;

    /* handle the factor 2 separately */
    uint64_t e = 0;
    while ((m & 1ULL) == 0) {
        m >>= 1;
        ++e;
    }
    if (e > 0) {
        result *= (pow_seq((seq_t)2, (idx_t)(2 * e + 1)) - (seq_t)3);
    }

    /* odd factors */
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            uint64_t ee = 0;
            while (m % p == 0) {
                m /= p;
                ++ee;
            }
            seq_t num = pow_seq((seq_t)p, (idx_t)(2 * ee + 1)) - (seq_t)1;
            seq_t den = (seq_t)(p - 1);
            result *= num / den;
        }
    }

    /* remaining prime factor, if any */
    if (m > 1) {
        seq_t num = pow_seq((seq_t)m, (idx_t)3) - (seq_t)1; /* exponent 2*1+1 = 3 */
        seq_t den = (seq_t)(m - 1);
        result *= num / den;
    }

    return result;
}
