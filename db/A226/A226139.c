/*
 * A226139  Expansion of b(-q) * b(q^2) in powers of q where b() is a cubic
 *          AGM theta function.
 * https://oeis.org/A226139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3 * b(n) where b(n) is multiplicative with • b(2^e) = -1
 *            • b(3^e) = 4 - 3^(e+1) • for p > 3, b(p^e) = (p^(e+1)-1) / (p-1)
 *            (exact division) (And b(1) = 1.) This implementation factors n
 *            by trial division, computes the multiplicative factors using
 *            integer exponentiation, and multiplies them...
 * Range:     i8 n <= 12, i16 n <= 906, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Multiplies two seq_t values.  No overflow handling is required here
   because the build may provide an arbitrary precision type. */
static seq_t mul_seq(seq_t a, seq_t b) { return a * b; }

/* Integer exponentiation by squaring.  exp is an idx_t (unsigned). */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0) {
        if (exp & 1) result = mul_seq(result, base);
        base = mul_seq(base, base);
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0U) return (seq_t)1;          /* a(0) = 1 */

    seq_t b = (seq_t)1;                     /* multiplicative accumulator */
    idx_t m = n;

    /* trial division to factor m */
    for (idx_t p = 2U; p * p <= m; ++p) {
        if (m % p == 0U) {
            idx_t e = 0U;
            while (m % p == 0U) {
                m /= p;
                ++e;
            }

            seq_t term;
            if (p == 2U) {
                term = (seq_t)-1;                     /* b(2^e) = -1 */
            } else if (p == 3U) {
                seq_t pow3 = pow_seq((seq_t)3, e + 1U); /* 3^(e+1) */
                term = (seq_t)4 - pow3;                 /* 4 - 3^(e+1) */
            } else {
                seq_t powp = pow_seq((seq_t)p, e + 1U);   /* p^(e+1) */
                term = (powp - (seq_t)1) / ((seq_t)(p - 1U));
            }
            b = mul_seq(b, term);
        }
    }

    /* remaining prime factor > sqrt(original m) */
    if (m > 1U) {
        idx_t p = m;
        seq_t term;
        if (p == 2U) {
            term = (seq_t)-1;
        } else if (p == 3U) {
            seq_t pow3 = pow_seq((seq_t)3, 2U);   /* 3^(1+1) */
            term = (seq_t)4 - pow3;
        } else {
            seq_t powp = pow_seq((seq_t)p, 2U);   /* p^(1+1) */
            term = (powp - (seq_t)1) / ((seq_t)(p - 1U));
        }
        b = mul_seq(b, term);
    }

    return 3 * b;   /* a(n) = 3 * b(n) */
}
