/*
 * A040558  Continued fraction for sqrt(583).
 * https://oeis.org/A040558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction for sqrt(583)
   a(0) = 24
   a(1).. periodic with period 10: 6,1,7,5,4,5,7,1,6,48 */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* a(0) is the integer part of sqrt(583) */
    if (n == 0)
        return (seq_t)24;  /* floor_sqrt(583) */

    static seq_t period[200];
    static idx_t period_len = 0;
    static bool initialized = false;

    if (!initialized)
    {
        const seq_t N   = (seq_t)583;
        const seq_t a0  = floor_sqrt(N);          /* 24 */
        seq_t m = (seq_t)0;
        seq_t d = (seq_t)1;
        seq_t a = a0;

        period_len = 0;
        for (;;)
        {
            /* recurrence for continued fraction of sqrt(N) */
            m = d * a - m;
            d = (N - m * m) / d;
            a = (a0 + m) / d;

            period[period_len++] = a;
            if (a == a0 * (seq_t)2)   /* last term of the period */
                break;
        }
        initialized = true;
    }

    /* periodic indices start at n==1 */
    idx_t idx = (n - 1) % period_len;
    return period[(size_t)idx];
}
