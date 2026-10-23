/*
 * A117255  Column 0 of triangle A117254.
 * https://oeis.org/A117255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = [∏_{j=0..n-1} (1-4*j)]/n! * 4^(n*(n-1)/2) Recurrence
 *            used: a(0) = 1 a(n+1) = a(n) * (1-4*n) * 4^n / (n+1)
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 5, i64 n <= 7, i128 n <= 10,
 *            big any n
 * Helpers:   pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow4.h"   /* seq_t pow4(idx_t exp) */

seq_t a(idx_t n)
{
    seq_t res = 1;
    for (idx_t i = 0; i < n; ++i)
    {
        seq_t factor = (seq_t)1 - (seq_t)4 * (seq_t)i;
        res *= factor;
        res *= pow4(i);
        res /= (seq_t)(i + 1);
    }
    return res;
}
