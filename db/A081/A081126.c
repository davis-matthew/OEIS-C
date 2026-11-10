/*
 * A081126  Duplicate of A018191.
 * https://oeis.org/A081126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * k! / floor(k/2)!
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 14, i64 n <= 22, i128 n <= 22,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 22 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t term = (seq_t)1;
    seq_t sum  = term;

    for (idx_t k = 0; k < n; ++k) {
        term = term * ((seq_t)(n - k));
        if (k & 1U) {
            idx_t divisor = (k + 1) / 2;
            term = term / ((seq_t)divisor);
        }
        sum += term;
    }

    return sum;
}
