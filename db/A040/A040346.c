/*
 * A040346  Continued fraction for sqrt(366).
 * https://oeis.org/A040346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) is the n-th term of the simple continued fraction expansion
 *            of sqrt(366). The expansion is
 *            [19;overline{7,1,1,1,2,12,2,1,1,1,7,38}]
 * Range:     u8 none, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static seq_t period_arr[32];
static size_t period_len = 0;
static int period_init = 0;

seq_t a(idx_t n)
{
    const seq_t a0 = 19; /* floor(sqrt(366)) */

    if (!period_init) {
        const seq_t D = 366;
        seq_t m = 0;
        seq_t d = 1;
        seq_t a = a0;
        size_t len = 0;

        /* Generate the periodic part until the term 2*a0 appears */
        while (1) {
            m = d * a - m;
            d = (D - m * m) / d;
            a = (a0 + m) / d;
            period_arr[len++] = a;
            if (a == a0 * 2) {
                break;
            }
        }
        period_len = len;
        period_init = 1;
    }

    if (n == 0) {
        return a0;
    }

    /* Return the element in the period corresponding to n */
    size_t idx = (size_t)((n - 1) % period_len);
    return period_arr[idx];
}
