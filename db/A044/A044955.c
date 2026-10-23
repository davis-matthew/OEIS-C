/*
 * A044955  Numbers with no two equally numerous base 6 digits.
 * https://oeis.org/A044955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers with no two equally numerous base‑6 digits
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   distinct_counts_base6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/distinct_counts_base6.h"

seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t cur   = 1;
    while (found < n) {
        if (distinct_counts_base6(cur))
            ++found;
        if (found == n)
            break;
        ++cur;
    }
    return cur;
}
