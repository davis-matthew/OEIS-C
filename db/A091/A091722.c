/*
 * A091722  Babylonian sexagesimal (base 60) expansion of 1/13.
 * https://oeis.org/A091722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = {4,36,55,23} repeating with period 4 */
/* implemented by a simple lookup using n mod 4 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* period of the sexagesimal expansion of 1/13 */
    static const seq_t period[4] = {4, 36, 55, 23};
    /* take n modulo 4 – idx_t is unsigned */
    return period[n & 3U];
}
