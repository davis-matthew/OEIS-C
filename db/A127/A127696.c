/*
 * A127696  a(n) = (2*n)^n+(2*n+1)^n-(2*n+2)^n.
 * https://oeis.org/A127696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (2*n)^n + (2*n+1)^n - (2*n+2)^n
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 8, i64 n <= 14, i128 n <= 23,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t two_n = (seq_t)n * 2;
    seq_t val1  = seq_pow(two_n, n);
    seq_t val2  = seq_pow(two_n + 1, n);
    seq_t val3  = seq_pow(two_n + 2, n);

    return val1 + val2 - val3;
}
