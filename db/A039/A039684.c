/*
 * A039684  Erroneous version of A001232.
 * https://oeis.org/A039684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 1100*10^(n-1) - 11
 * Range:     i8 none, i16 n <= 2, i32 n <= 6, i64 n <= 6, i128 n <= 6,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* offset 1; behavior below offset irrelevant */
    seq_t p = pow10(n - 1);     /* 10^(n-1) */
    seq_t res = p * (seq_t)1100 - (seq_t)11;
    return res;
}
