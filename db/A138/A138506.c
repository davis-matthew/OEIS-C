/*
 * A138506  Expansion of f(q)^5 / f(q^5) in powers of q where f() is a
 *          Ramanujan theta function.
 * https://oeis.org/A138506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   Multiplicative definition for n≥1: a(n) = 5 * b(n) b(2^e) =
 *            ((-2)^(e+1) – 1) / 3 b(p^e) = ((-p)^(e+1) – 1) / (–p – 1) if p ≡
 *            2 or 3 (mod 5) b(p^e) = (p^(e+1) – 1) / (p – 1) if p ≡ 1 or 4
 *            (mod 5) (p ≠ 5) b(5^e) = 1 a(0) = 1
 * Range:     i8 n <= 12, i16 n <= 606, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/*  Return the n‑th term of OEIS A138506.  */
seq_t a(idx_t n)
{
    if (n == 0) return 1;

    seq_t res = 1;
    uint64_t m = (uint64_t)n;
    uint64_t p;

    for (p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }

            seq_t factor;
            if (p == 2) {
                seq_t pow = 1;
                seq_t base = -((seq_t)2);
                for (int i = 0; i < exp + 1; ++i) pow *= base;
                factor = (pow - 1) / ((seq_t)3);
            } else if (p == 5) {
                factor = 1;
            } else if ((p % 5 == 2) || (p % 5 == 3)) {
                seq_t pow = 1;
                seq_t base = -((seq_t)p);
                for (int i = 0; i < exp + 1; ++i) pow *= base;
                seq_t denom = -((seq_t)p) - 1;
                factor = (pow - 1) / denom;
            } else { /* p%5==1 or p%5==4 */
                seq_t pow = 1;
                seq_t base = (seq_t)p;
                for (int i = 0; i < exp + 1; ++i) pow *= base;
                seq_t denom = (seq_t)p - 1;
                factor = (pow - 1) / denom;
            }

            res *= factor;
        }
    }

    /* remaining prime factor, if any */
    if (m > 1) {
        p = m;          /* m is prime */
        int exp = 1;

        seq_t factor;
        if (p == 2) {
            seq_t pow = 1;
            seq_t base = -((seq_t)2);
            for (int i = 0; i < exp + 1; ++i) pow *= base;
            factor = (pow - 1) / ((seq_t)3);
        } else if (p == 5) {
            factor = 1;
        } else if ((p % 5 == 2) || (p % 5 == 3)) {
            seq_t pow = 1;
            seq_t base = -((seq_t)p);
            for (int i = 0; i < exp + 1; ++i) pow *= base;
            seq_t denom = -((seq_t)p) - 1;
            factor = (pow - 1) / denom;
        } else { /* p%5==1 or p%5==4 */
            seq_t pow = 1;
            seq_t base = (seq_t)p;
            for (int i = 0; i < exp + 1; ++i) pow *= base;
            seq_t denom = (seq_t)p - 1;
            factor = (pow - 1) / denom;
        }

        res *= factor;
    }

    return 5 * res;
}
