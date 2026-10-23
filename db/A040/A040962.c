/*
 * A040962  Continued fraction for sqrt(994).
 * https://oeis.org/A040962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   compute_period
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction for sqrt(994) */
/* a(0) = floor(sqrt(994)) = 31 */
/* a(n) for n>=1 is the (n-1)‑th element of the repeating period [1,1,8,1,1,62] */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/compute_period.h"

/* external integer types supplied by the build */
extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t a0;
    static seq_t period[16];
    static size_t period_len;

    if (!initialized) {
        compute_period(period, &period_len, &a0);
        initialized = true;
    }

    if (n == 0)
        return a0;

    size_t idx = (size_t)(n - 1) % period_len;
    return period[idx];
}
