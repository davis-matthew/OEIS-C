/*
 * A014296  Inverse of 287th cyclotomic polynomial.
 * https://oeis.org/A014296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) + a(n-287) - a(n-288) + c(n), * where c(0)=1,
 *            c(7)=−1, c(41)=−1, c(48)=1, and c(n)=0 otherwise. * The sequence
 *            is periodic with period 287
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static bool init = false;
    static seq_t *period = NULL;          /* holds a(0)..a(286) */
    if (!init)
    {
        const size_t P = 287;             /* period length */
        period = (seq_t*)malloc(P * sizeof(seq_t));
        if (!period) return (seq_t)0;     /* out-of-memory guard */

        /* Compute the first 287 terms using the recurrence above */
        for (size_t i = 0; i < P; ++i)
        {
            seq_t c = 0;
            if (i == 0)      c = 1;
            else if (i == 7) c = -1;
            else if (i == 41)c = -1;
            else if (i == 48)c = 1;

            seq_t a_nm1   = (i >= 1)   ? period[i - 1]   : 0;
            seq_t a_nm287 = (i >= 287) ? period[i - 287] : 0;
            seq_t a_nm288 = (i >= 288) ? period[i - 288] : 0;

            period[i] = c + a_nm1 + a_nm287 - a_nm288;
        }
        init = true;
    }
    idx_t idx = n % 287;
    return period[idx];
}
