/*
 * A058552  Numerators of q-expansion of Eisenstein series E_16(q).
 * https://oeis.org/A058552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 16320 * σ₁₅(n) (σ₁₅ is the sum of the 15th powers of the
 *            divisors)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Helper: compute base^exp where base is uint64_t, exp fits in unsigned int */
static inline seq_t pow_seq_u64(uint64_t base, unsigned int exp)
{
    seq_t result = 1;
    seq_t b = (seq_t)base;
    for (unsigned int i = 0; i < exp; ++i)
        result *= b;
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;              /* a(0) is defined as 1 */

    uint64_t m = (uint64_t)n;        /* n fits in uint64_t for all idx_t */
    seq_t sigma = 1;

    /* Factor n by trial division */
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p != 0)
            continue;

        /* Count exponent of prime p */
        int e = 0;
        while (m % p == 0) {
            m /= p;
            ++e;
        }

        /* Compute p^15 once */
        seq_t p15 = pow_seq_u64(p, 15);

        /* Compute term = 1 + p^15 + p^30 + ... + p^(15e) */
        seq_t term = 0;
        seq_t cur = 1;
        for (int i = 0; i <= e; ++i) {
            term += cur;
            cur *= p15;
        }

        sigma *= term;
    }

    /* If after loop m > 1, then m is a prime factor > sqrt(original n) */
    if (m > 1) {
        /* exponent e = 1 */
        seq_t p15 = pow_seq_u64(m, 15);
        seq_t term = 1 + p15;      /* 1 + m^15 */
        sigma *= term;
    }

    /* Multiply by the constant 16320 */
    return (seq_t)16320 * sigma;
}
