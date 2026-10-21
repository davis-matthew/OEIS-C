/*
 * A070751  Numbers n such that sin(n) < 0.
 * https://oeis.org/A070751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A070751: numbers n such that sin(n) < 0.
   The implementation simply scans integers m=1,2,3,… and counts the
   indices where sin(m) (in radians) is negative, returning the nth
   such integer.  It relies on the double‑precision sin() function. */

#include <math.h>

seq_t a(idx_t n)
{
    idx_t count = 0;     /* how many negative-sine values found */
    idx_t m = 1;         /* current integer to test */

    for (;; ++m) {
        /* sin() accepts a double; cast harmlessly since m fits in double */
        if (sin((double)m) < 0.0) {
            ++count;
            if (count == n) {
                return (seq_t)m;   /* found the nth element */
            }
        }
    }
}
