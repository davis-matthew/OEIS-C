/*
 * A169925  Integer values of 4*n-40+360/(n+9) (see A169924).
 * https://oeis.org/A169925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 4*(d-9)-40+360/d; where d is the nth divisor of 360 that
 *            is ≥ 9. The sequence starts with n = 0 corresponding to d = 9,
 *            so that a(1) = 0, a(2) = 0, … No hard‑coded table of the
 *            sequence values is used
 * Range:     u8 n <= 13, u16 n <= 17, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Cache the 17 terms once. */
    static seq_t cache[17];
    static bool ready = false;

    if (!ready)
    {
        size_t i = 0;
        for (idx_t d = 9; d <= 360; ++d)
        {
            if (360 % d == 0)           /* d is a divisor of 360 */
            {
                seq_t val = (seq_t)4 * (seq_t)d - (seq_t)76 + (seq_t)(360 / d);
                cache[i] = val;
                ++i;
            }
        }
        ready = true;
    }

    if (n == 0 || n > 17) return 0;   /* out of bounds; sequence is finite   */
    return cache[n - 1];
}
