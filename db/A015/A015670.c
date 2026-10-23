/*
 * A015670  Inverse of 1661st cyclotomic polynomial.
 * https://oeis.org/A015670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A015670: Inverse of the 1661st cyclotomic polynomial.
   The coefficients a(n) satisfy a(0)=1; for n≥1
   a(n) = a(n‑1) + c(n) with
       c(n) =  1  if n = 162
       c(n) = -1  if n = 11 or 151
       c(n) =  0  otherwise
   The sequence is periodic with period 1661. */
#include <stddef.h>

#define PER 1661

seq_t a(idx_t n)
{
    /* static buffer for one period */
    static seq_t cache[PER];
    static int init = 0;

    if (!init) {
        init = 1;
        /* first term */
        cache[0] = 1;
        /* build the remaining terms of one period */
        for (size_t i = 1; i < PER; ++i) {
            seq_t add = 0;
            if (i == 11 || i == 151)
                add = -1;
            else if (i == 162)
                add = 1;
            cache[i] = cache[i - 1] + add;
        }
    }

    /* periodicity: return the appropriate entry */
    size_t idx = (size_t)(n % PER);
    return cache[idx];
}
