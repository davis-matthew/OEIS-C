/*
 * A040944  Continued fraction for sqrt(976).
 * https://oeis.org/A040944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction for sqrt(976)
   a(0) = floor(sqrt(976)) = 31
   For n >= 1, a(n) repeats with period 18:
   4, 6, 1, 2, 3, 1, 4, 2, 3, 2, 4, 1, 3, 2, 1, 6, 4, 62
   This implementation computes the period once and returns a(n) in O(1) after that. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* static cache of period */
    static seq_t period[2000];
    static size_t period_len = 0;
    static seq_t a0 = 0;

    if (n == 0) {
        /* compute a0 first time */
        if (a0 == 0)
            a0 = floor_sqrt((seq_t)976);
        return a0;
    }

    /* compute period once */
    if (period_len == 0) {
        seq_t m = 0;
        seq_t d = 1;
        seq_t a = a0;
        /* generate until a == 2*a0 */
        while (1) {
            m = d * a - m;
            d = (seq_t)((976 - m * m) / d);
            a = (a0 + m) / d;
            period[period_len++] = a;
            if (a == 2 * a0)            /* period ends here */
                break;
        }
    }

    /* return term (n-1)-th in period */
    return period[(n - 1) % period_len];
}
