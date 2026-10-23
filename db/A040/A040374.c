/*
 * A040374  Continued fraction for sqrt(394).
 * https://oeis.org/A040374
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=19; for n>0 the continued‑fraction coefficients for √394
 *            repeat the period (1,5,1,1,1,3,1,3,5,2,2,5,3,1,3,1,1,1,5,1,38)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

static const seq_t a0 = 19;
static const seq_t period[] = {
    1,5,1,1,1,3,1,3,5,2,2,5,3,1,3,1,1,1,5,1,38
};
static const idx_t period_len = sizeof(period) / sizeof(period[0]);

seq_t a(idx_t n)
{
    if (n == 0)
        return a0;
    idx_t idx = (n - 1) % period_len;
    return period[(size_t)idx];
}
