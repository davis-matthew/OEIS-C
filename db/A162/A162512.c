/*
 * A162512  Dirichlet inverse of A162511.
 * https://oeis.org/A162512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, mult, sign
 * Formula:   not stated
 * Range:     i8 n <= 255, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dirichlet inverse of A162511
   multiplicative: a(p^e) = (-1)^e * 2^(e‑1)  (for all primes p, e≥1)  */
#include <stdint.h>

seq_t a(idx_t n) {
    /* a(1) = 1 */
    if (n <= 1) return 1;

    seq_t result = 1;
    uint64_t temp = (uint64_t)n;

    /* handle factor 2 */
    if ((temp & 1) == 0) {
        uint64_t e = 0;
        while ((temp & 1) == 0) {
            temp >>= 1;
            ++e;
        }
        seq_t pow2 = 1;
        for (uint64_t i = 1; i < e; ++i) pow2 *= 2;      /* 2^(e‑1) */
        seq_t term = (e & 1ULL) ? -pow2 : pow2;           /* (-1)^e * 2^(e‑1) */
        result *= term;
    }

    /* handle odd prime factors */
    for (uint64_t d = 3; d <= temp / d; d += 2) {
        if (temp % d == 0) {
            uint64_t e = 0;
            while (temp % d == 0) {
                temp /= d;
                ++e;
            }
            seq_t pow2 = 1;
            for (uint64_t i = 1; i < e; ++i) pow2 *= 2;
            seq_t term = (e & 1ULL) ? -pow2 : pow2;
            result *= term;
        }
    }

    /* remaining prime factor (exponent 1) */
    if (temp > 1) {
        result *= (seq_t)(-1);   /* e = 1 → (-1)^1 * 1 = -1 */
    }

    return result;
}
