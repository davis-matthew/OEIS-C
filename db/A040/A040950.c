/*
 * A040950  Continued fraction for sqrt(982).
 * https://oeis.org/A040950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction for sqrt(982)
   a(0) = ⌊√982⌋ = 31
   a(1) = 2, a(2) = 1, a(3) = 30, a(4) = 1, a(5) = 2, a(6) = 62
   and then the period 2,1,30,1,2,62 repeats. */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/int_sqrt.h"

seq_t a(idx_t n)
{
    static bool initialized = false;
    static idx_t a0 = 0;                 /* integer part of √982 */
    static idx_t period[30];             /* period of the continued fraction */
    static size_t period_len = 0;        /* length of the period         */

    if (!initialized) {
        const idx_t D = 982;
        idx_t m  = 0;
        idx_t d  = 1;
        idx_t a_cur = int_sqrt(D);       /* floor(√982)  */
        a0 = a_cur;

        /* compute the period */
        do {
            m = d * a_cur - m;
            d = (D - m * m) / d;
            a_cur = (a0 + m) / d;
            period[period_len++] = a_cur;
        } while (a_cur != 2 * a0);      /* period ends when a == 2*a0 */

        initialized = true;
    }

    if (n == 0) {
        return (seq_t)a0;
    } else {
        return (seq_t)period[(n - 1) % period_len];
    }
}
