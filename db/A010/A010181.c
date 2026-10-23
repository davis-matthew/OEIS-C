/*
 * A010181  Continued fraction for sqrt(116).
 * https://oeis.org/A010181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) equals the n-th partial quotient in the continued fraction
 *            expansion of sqrt(116). The first term a(0) is floor(sqrt(116))
 *            and the subsequent terms repeat with period 10
 * Range:     u8 none, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    static seq_t a0_val = 0;            /* floor(sqrt(116)) */
    static seq_t period[10];           /* periodic part a(1)..a(10) */
    static size_t period_len = 0;      /* number of periodic terms */

    if (period_len == 0) {             /* compute period once */
        const seq_t D = (seq_t)116;
        a0_val = floor_sqrt(D);

        seq_t m = 0;
        seq_t d = 1;
        seq_t a_cur = a0_val;
        size_t idx = 0;

        while (1) {
            m = d * a_cur - m;
            d = (D - m * m) / d;
            a_cur = (a0_val + m) / d;

            period[idx++] = a_cur;
            if (a_cur == 2 * a0_val)   /* 2*floor(sqrt(116)) signals end of period */
                break;
        }
        period_len = idx;              /* should be 10 for sqrt(116) */
    }

    if (n == 0)
        return a0_val;                  /* a(0) */

    /* Index into the periodic part: a(1) is period[0] */
    size_t idx = (size_t)((n - 1) % (idx_t)period_len);
    return period[idx];
}
