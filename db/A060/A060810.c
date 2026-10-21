/*
 * A060810  Numbers whose squares have their digits in strictly increasing
 *          order.
 * https://oeis.org/A060810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, full, nonn, fini
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 14, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   increasing_square
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = the n‑th integer x whose square x² has digits in strictly increasing order. */

#include <stdbool.h>
#include "../utils/increasing_square.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;               /* offset starts at 1 */
    idx_t count = 0;
    seq_t val = 0;
    while (true) {
        if (increasing_square(val)) {
            ++count;
            if (count == n)
                return val;
        }
        ++val;
    }
}
