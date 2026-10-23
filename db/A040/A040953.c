/*
 * A040953  Continued fraction for sqrt(985).
 * https://oeis.org/A040953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = floor(sqrt(985)) = 31. For n >= 1, a(n) are the periodic
 *            partial quotients of the continued fraction expansion of
 *            sqrt(985). The period is [2,1,1,2,62], so a(n) = 2,1,1,2,62
 *            repeatedly starting from n = 1
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static variables are initialized only once */
    static bool initialized = false;
    static seq_t a0 = 0;                   /* integer part floor(sqrt(985)) */
    static seq_t period[128] = {0};        /* periodic part after a(0) */
    static idx_t period_len = 0;           /* length of the period */

    if (!initialized) {
        /* compute integer part a0 of sqrt(985) by binary search */
        const unsigned int D = 985u;
        unsigned int low = 0, high = 1;
        while (high * high <= D) high *= 2;
        while (low + 1 < high) {
            unsigned int mid = (low + high) / 2;
            if (mid * mid <= D)
                low = mid;
            else
                high = mid;
        }
        a0 = (seq_t)low;

        /* compute the periodic part of the continued fraction */
        unsigned long long m = 0;
        unsigned long long d = 1;
        unsigned long long a = low;  /* use the integer part */
        while (1) {
            m = d * a - m;
            d = (D - m * m) / d;
            a = (low + m) / d;
            period[period_len++] = (seq_t)a;
            if (a == 2 * low)     /* end of period */
                break;
        }
        initialized = true;
    }

    if (n == 0)
        return a0;

    /* n >= 1 : periodic part */
    return period[(n - 1) % period_len];
}
