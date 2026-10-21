/*
 * A214456  Expansion of b(q^2) * (b(q) + 2 * b(q^4)) / 3 in powers of q where
 *          b() is a cubic AGM theta function.
 * https://oeis.org/A214456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -∏_{p^e∥n} b(p^e) with b(2^e) = 3, b(3^e) = 4 – 3^(e+1),
 *            b(p^e) = (p^(e+1) – 1) / (p – 1) for p > 3 (and a(0) = 1). This
 *            implements the multiplicative definition of A214456
 * Range:     i8 n <= 12, i16 n <= 316, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static seq_t pow_int(seq_t base, int exp)
{
    seq_t result = 1;
    for (int i = 0; i < exp; ++i) {
        result *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    uint64_t m = (uint64_t)n;
    seq_t term = 1;

    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            seq_t val;
            if (p == 2) {
                val = (seq_t)3;
            } else if (p == 3) {
                val = (seq_t)4 - pow_int((seq_t)3, e + 1);
            } else {
                val = (pow_int((seq_t)p, e + 1) - (seq_t)1) /
                      ((seq_t)p - (seq_t)1);
            }
            term *= val;
        }
    }

    if (m > 1) {                 /* m is a prime factor > sqrt(original n) */
        uint64_t p = m;
        int e = 1;
        seq_t val;
        if (p == 2) {
            val = (seq_t)3;
        } else if (p == 3) {
            val = (seq_t)4 - pow_int((seq_t)3, e + 1);
        } else {
            val = (pow_int((seq_t)p, e + 1) - (seq_t)1) /
                  ((seq_t)p - (seq_t)1);
        }
        term *= val;
    }

    return -term;
}
