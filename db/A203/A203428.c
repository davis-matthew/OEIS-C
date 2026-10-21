/*
 * A203428  Reciprocal of Vandermonde determinant of (1/3,1/6,...,1/(3n)).
 * https://oeis.org/A203428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 4, i64 n <= 6, i128 n <= 8,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1; for n ≥ 1:
 *     a(n+1) = a(n) * (-3)^n * (n+1)^n
 * This follows from the closed form
 * a(n) = (-3)^(n(n-1)/2) * (n!)^(n-1) / BarnesG(n+1). */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)            /* offset 1 – undefined for 0 */
        return 0;

    seq_t res = 1;
    for (idx_t i = 1; i <= n - 1; ++i) {
        seq_t pow_neg3 = big_pow((seq_t)-3, (seq_t)i);
        seq_t pow_ip1 = big_pow((seq_t)(i + 1), (seq_t)i);
        res *= pow_neg3;
        res *= pow_ip1;
    }
    return res;
}
