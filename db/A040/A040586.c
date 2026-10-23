/*
 * A040586  Continued fraction for sqrt(611).
 * https://oeis.org/A040586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Continued fraction of sqrt(611).
    a(0) = 24
    For n >= 1, the terms repeat with period 14:
    [1, 2, 1, 1, 4, 2, 1, 2, 4, 1, 1, 2, 1, 48] */

/*  seq_t a(idx_t n)  */
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* period of the continued fraction of sqrt(611) */
    static bool initialized = false;
    static seq_t period[100];
    static idx_t period_len = 0U;

    if (!initialized) {
        const idx_t D   = 611U;   /* discriminant */
        const idx_t a0  = 24U;    /* floor(sqrt(D)) */
        idx_t m = 0U, d = 1U, a = a0;
        idx_t idx = 0U;

        while (1) {
            m = d * a - m;
            d = (D - m * m) / d;
            a = (a0 + m) / d;
            period[idx++] = (seq_t)a;
            if (a == 2U * a0)   /* period ends when a = 2*a0 */
                break;
        }
        period_len = idx;
        initialized = true;
    }

    if (n == 0U)
        return (seq_t)24U; /* a(0) = 24 */

    idx_t pos = (n - 1U) % period_len;
    return period[pos];
}
