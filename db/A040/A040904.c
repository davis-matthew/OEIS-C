/*
 * A040904  Continued fraction for sqrt(935).
 * https://oeis.org/A040904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = n-th term of the continued fraction expansion of
 *            sqrt(935)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Period: 1,1,2,1,2,1,1,60 (length 8) */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* constants for sqrt(935) */
    const idx_t D     = 935;   /* D = 935 */
    const idx_t a0int = 30;    /* floor(sqrt(935)) = 30 */
    static seq_t period[128];
    static size_t period_len = 0;

    /* compute the period only once */
    if (period_len == 0) {
        idx_t m = 0;
        idx_t d = 1;
        idx_t a = a0int;
        while (1) {
            /* next step of continued fraction algorithm */
            m = d * a - m;
            d = (D - m * m) / d;
            a = (a0int + m) / d;
            period[period_len++] = (seq_t)a;
            if (a == 2 * a0int) /* end of the period */
                break;
        }
    }

    if (n == 0)            /* first term */
        return (seq_t)a0int;

    size_t idx = (size_t)((n - 1) % period_len); /* index into period */
    return period[idx];
}
