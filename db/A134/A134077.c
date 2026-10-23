/*
 * A134077  Expansion of psi(x) * phi(-x)^3 / chi(-x^3)^3 in powers of x where
 *          phi(), psi(), chi() are Ramanujan theta functions.
 * https://oeis.org/A134077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = b(2*n+1), where b is multiplicative and b(2^e) = 0^e,
 *            b(3^e) = 4 - 3^(e+1), b(p^e) = (p^(e+1)-1)/(p-1) for p>5 (note
 *            that 2*n+1 is always odd, so the 2^e rule never applies)
 * Range:     i8 n <= 5, i16 n <= 229, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute m = 2*n + 1 as a seq_t then cast to idx_t for trial division.
       This works safely for all n that fit in idx_t without overflow,
       which covers the intended usage range (n up to about 10000). */
    seq_t m_seq = (seq_t)n * 2 + 1;
    idx_t m = (idx_t)m_seq;          /* safe because 2*n+1 < 2^64 for n < 2^63 */

    seq_t result = 1;
    idx_t temp = m;

    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }

            seq_t term;
            if (p == 3) {
                /* 4 - 3^(e+1) */
                seq_t pow = 1;
                for (idx_t i = 0; i < e + 1; ++i) pow *= 3;
                term = (seq_t)4 - pow;
            } else {
                /* (p^(e+1)-1)/(p-1) */
                seq_t pow = 1;
                for (idx_t i = 0; i < e + 1; ++i) pow *= (seq_t)p;
                term = (pow - 1) / ((seq_t)p - 1);
            }
            result *= term;
        }
    }

    if (temp > 1) { /* remaining prime factor > sqrt(original m) */
        idx_t p = temp;
        idx_t e = 1;

        seq_t term;
        if (p == 3) {
            seq_t pow = 1;
            for (idx_t i = 0; i < e + 1; ++i) pow *= 3;
            term = (seq_t)4 - pow;
        } else {
            seq_t pow = 1;
            for (idx_t i = 0; i < e + 1; ++i) pow *= (seq_t)p;
            term = (pow - 1) / ((seq_t)p - 1);
        }
        result *= term;
    }

    return result;
}
