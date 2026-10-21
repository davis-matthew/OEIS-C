/*
 * A091721  Babylonian sexagesimal (base 60) expansion of 1/11.
 * https://oeis.org/A091721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n+5) = a(n)  (period 5).  The values are the base‑60 digits of 1/11.
   They are computed from the definition: start with remainder r = 1,
   then for each position multiply r by 60, take the integer part
   `(r*60)/11` as the digit, and replace r by `(r*60) % 11`.  This gives
   the periodic block [5, 27, 16, 21, 49].  The block is computed once
   and then reused for all indices. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* one‑time computation of the period 5 block */
    static seq_t period[5];
    static int initialized = 0;

    if (!initialized) {
        seq_t r = 1;          /* current remainder modulo 11 */
        for (int i = 0; i < 5; ++i) {
            period[i] = (r * (seq_t)60) / 11;   /* digit */
            r = (r * (seq_t)60) % 11;           /* new remainder */
        }
        initialized = 1;
    }

    /* return the appropriate element of the periodic block */
    return period[(size_t)(n % 5)];
}
