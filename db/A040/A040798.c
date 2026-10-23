/*
 * A040798  Continued fraction for sqrt(827).
 * https://oeis.org/A040798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   continued fraction for sqrt(827)
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = floor(sqrt(827)) = 28 */
/* the periodic part is of length 10: 1,3,7,1,27,1,7,3,1,56 */

#include <stdbool.h>
#include <stddef.h>

static const idx_t D   = 827ULL;
static const idx_t a0  = 28ULL;

static bool          initialized  = false;
static seq_t         period[20];
static size_t        period_len   = 0;

seq_t a(idx_t n)
{
    /* a(0) is the integer part of sqrt(827) */
    if (n == 0)
        return (seq_t)a0;

    /* initialise the period only once */
    if (!initialized) {
        idx_t m = 0;
        idx_t d = 1;
        idx_t a = a0;
        /* generate terms until the first occurrence of 2*a0,
           which marks the end of the period for a continued fraction */
        do {
            m = d * a - m;
            d = (D - m * m) / d;
            a = (a0 + m) / d;
            period[period_len++] = (seq_t)a;
        } while (a != 2 * a0);
        initialized = true;
    }

    /* the remaining terms repeat periodically */
    size_t pos = (size_t)((n - 1) % period_len);
    return period[pos];
}
