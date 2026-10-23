/*
 * A140277  Rounded (first-quadrant) angle in degrees whose tangent is n.
 * https://oeis.org/A140277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(180 * atan(n) / Pi)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

static seq_t table[115];
static int initialized = 0;

/* Lazily initialize lookup table for 0 ≤ n < 115 using double arithmetic.
   The values are small, so double precision is more than enough to
   correctly round to the nearest integer. */
static void init_table(void)
{
    const double pi = acos(-1.0);
    for (idx_t i = 0; i < 115; ++i) {
        double ang = atan((double)i);
        double deg = ang * 180.0 / pi;
        /* llround gives the nearest integer; ties away from zero which matches
           the specification for .5000… rounding up. */
        table[i] = (seq_t)llround(deg);
    }
    initialized = 1;
}

seq_t a(idx_t n)
{
    if (!initialized)
        init_table();

    if (n < 115)
        return table[n];
    else
        return (seq_t)90;
}
