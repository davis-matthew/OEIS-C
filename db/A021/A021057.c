/*
 * A021057  Decimal expansion of 1/53.
 * https://oeis.org/A021057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A021057: decimal expansion of 1/53
   a(n) = floor((10^n mod 53) * 10 / 53)   for n >= 0
*/
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t digits[52];
    static int init = 0;

    if (!init) {
        uint64_t rem = 1;       /* current remainder of 1/53 */
        for (size_t i = 0; i < 52; ++i) {
            rem = rem * 10;                     /* shift next decimal digit */
            digits[i] = (seq_t)(rem / 53);      /* digit at position i */
            rem = rem % 53;                     /* new remainder */
        }
        init = 1;
    }

    return digits[(size_t)(n % 52)];
}
