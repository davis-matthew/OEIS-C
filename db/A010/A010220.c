/*
 * A010220  Continued fraction for sqrt(176).
 * https://oeis.org/A010220
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic continued fraction for sqrt(176) :
   a(0) = 13; for n ≥ 1, a(n) = {3,1,3,26} repeated. */

#include <stddef.h>
#include <stdbool.h>

static seq_t period[4];
static size_t period_len = 0;
static bool init_done = false;

seq_t a(idx_t n)
{
    /* Lazy initialization of the period */
    if (!init_done) {
        const idx_t N   = 176;
        const idx_t a0  = 13;          /* floor(sqrt(176)) */
        idx_t m = 0;
        idx_t d = 1;
        idx_t current = a0;

        /* generate the period until d == 1 */
        do {
            m = d * current - m;
            d = (N - m * m) / d;
            current = (a0 + m) / d;
            period[period_len++] = (seq_t)current;
        } while (d != 1);

        init_done = true;
    }

    if (n == 0)
        return 13;

    /* For n ≥ 1, use the periodic part modulo its length */
    idx_t idx = (n - 1) % period_len;
    return period[(size_t)idx];
}
