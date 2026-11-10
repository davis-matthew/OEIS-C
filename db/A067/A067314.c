/*
 * A067314  Duplicate of A066022.
 * https://oeis.org/A067314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = floor(n*log10(n)) + 1 (digits of n^n), a(0) = 1
 * Range:     i8 n <= 26, i16 n <= 26, i32 n <= 26, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 26 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    /* Powers of 10 give an exact integer n*log10(n); handle them exactly. */
    {
        idx_t m = n;
        unsigned k = 0;
        while (m % 10 == 0) { m /= 10; k++; }
        if (m == 1) {
            return (seq_t)n * (seq_t)k + 1;
        }
    }

    long double x = (long double)n * log10l((long double)n);
    unsigned long long d = (unsigned long long)x;
    return (seq_t)d + 1;
}
