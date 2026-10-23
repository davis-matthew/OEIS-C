/*
 * A044743  Numbers n such that string 3,0 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th integer whose decimal representation contains the
 *            adjacent digits 3 followed by 0, while the representation of (n
 *            + 1) does not contain that substring
 * Range:     u8 n <= 3, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   contains30
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/contains30.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; a(0) is undefined but harmless */
        return 0;

    seq_t x = 0;
    idx_t count = 0;

    for (;;) {
        ++x;                             /* examine the next integer */
        if (contains30(x) && !contains30(x + 1)) {
            ++count;
            if (count == n)
                return x;
        }
    }
}
