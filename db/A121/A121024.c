/*
 * A121024  Multiples of 4 containing a 4 in their decimal representation.
 * https://oeis.org/A121024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth positive multiple of 4 that contains a decimal
 *            digit 4
 * Range:     u8 n <= 19, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   contains_digit_4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/contains_digit_4.h"

seq_t a(idx_t n)
{
    if (n == 0) /* not part of the defined sequence, but returns 0 */
        return 0;

    seq_t candidate = 4;         /* first multiple of 4 containing a 4 is 4 itself */
    idx_t   count    = 1;        /* we already have the first term */

    while (count < n) {
        /* advance to the next multiple of 4 */
        do {
            candidate += 4;
        } while (!contains_digit_4(candidate));
        count++;
    }
    return candidate;
}
