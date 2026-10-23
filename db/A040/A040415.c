/*
 * A040415  Continued fraction for sqrt(436).
 * https://oeis.org/A040415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction coefficients of sqrt(436). */

#include <stddef.h>
#include <stdbool.h>

static const seq_t a0_value = (seq_t)20;
static seq_t period[128];
static size_t period_len = 0;
static bool initialized = false;

seq_t a(idx_t n)
{
    if (!initialized) {
        const uint64_t D = 436;
        uint64_t m = 0;
        uint64_t d = 1;
        uint64_t a0_64 = (uint64_t)a0_value;
        uint64_t a = a0_64;
        do {
            m = d * a - m;
            d = (D - m * m) / d;
            a = (a0_64 + m) / d;
            period[period_len++] = (seq_t)a;
        } while (a != 2 * a0_64);
        initialized = true;
    }

    if (n == 0) {
        return a0_value;
    }

    idx_t idx = (idx_t)((n - 1) % period_len);
    return period[idx];
}
