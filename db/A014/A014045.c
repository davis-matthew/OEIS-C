/*
 * A014045  Inverse of 36th cyclotomic polynomial.
 * https://oeis.org/A014045
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

/* Recurrence: a(n) = a(n-6) – a(n-12)  for n ≥ 12.
   Initial values: a(0)=1, a(1)=a(2)=a(3)=a(4)=a(5)=0, a(6)=1 (all others are 0).
   The sequence is periodic with period 36, corresponding to 1/(1-x^6+x^12). */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool init = false;
    static seq_t period[36];

    if (!init) {
        init = true;
        /* Zero the array (static storage already zero-initialized, but do it explicitly). */
        for (idx_t i = 0; i < 36; ++i) period[i] = 0;
        period[0] = 1;          /* a(0) */
        period[6] = 1;          /* a(6) */
        period[18] = -1;        /* a(18) will be overwritten by recurrence but keep for clarity */

        /* Compute remaining values up to 35 using the recurrence. */
        for (idx_t i = 12; i < 36; ++i)
            period[i] = period[i-6] - period[i-12];
    }

    return period[n % 36];      /* periodicity of length 36 */
}
