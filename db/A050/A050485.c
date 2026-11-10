/*
 * A050485  Duplicate of A034265.
 * https://oeis.org/A050485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = binomial(n+6,6)*(6*n+7)/7
 * Range:     i8 n <= 0, i16 n <= 5, i32 n <= 12, i64 n <= 12, i128 n <= 12,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 12 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t res = 1;
    for (seq_t i = 1; i <= 6; ++i)
    {
        seq_t term = n_seq + (7 - i);
        res = res * term;
        res = res / i;
    }
    seq_t mul = (seq_t)6 * n_seq + (seq_t)7;
    seq_t temp = res * mul;
    return temp / (seq_t)7;
}
