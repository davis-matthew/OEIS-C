/*
 * A040319  Continued fraction for sqrt(338).
 * https://oeis.org/A040319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   continued fraction coefficients of sqrt(338)
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>


seq_t a(idx_t n)
{
    /* Periodic coefficients after the leading term */
    static bool initialized = false;
    static seq_t period[10];
    static size_t period_len = 0;
    const idx_t D  = 338;
    const idx_t a0 = 18;      /* floor(sqrt(338)) */

    if (!initialized) {
        idx_t m = 0;
        idx_t d = 1;
        idx_t a = a0;
        size_t idx = 0;

        /* Generate the continued–fraction period */
        do {
            m = d * a - m;
            d = (D - m * m) / d;
            a = (a0 + m) / d;
            period[idx++] = (seq_t) a;
        } while (a != 2 * a0);      /* period finishes when a == 2*a0 */

        period_len = idx;
        initialized = true;
    }

    if (n == 0)
        return (seq_t) a0;

    return period[(size_t)(n - 1) % period_len];
}
