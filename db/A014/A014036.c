/*
 * A014036  Inverse of 27th cyclotomic polynomial.
 * https://oeis.org/A014036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n)+a(n-9)+a(n-18)=0 for n≥1, a(0)=1. Periodic with period 27. */
#include <stdbool.h>
seq_t a(idx_t n)
{
    static seq_t period[27];
    static bool init = false;
    if (!init)
    {
        for (idx_t i = 0; i < 27; ++i)
        {
            if (i == 0)
                period[i] = 1;
            else if (i < 9)
                period[i] = 0;
            else if (i < 18)
                period[i] = -period[i - 9];
            else
                period[i] = -period[i - 9] - period[i - 18];
        }
        init = true;
    }
    return period[n % 27];
}
