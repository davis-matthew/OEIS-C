/*
 * A044242  Numbers n such that string 6,7 occurs in the base 8 representation
 *          of n but not of n-1.
 * https://oeis.org/A044242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   sequence of n such that the string "6,7" occurs in the base‑8
 *            representation of n but not of n‑1
 * Range:     u8 n <= 4, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   has_no_67, has_substring_67
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_no_67.h"
#include "../utils/has_substring_67.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t m = 1;
    for (;;) {
        if (has_substring_67(m) && has_no_67(m - 1)) {
            ++count;
            if (count == n)
                return m;
        }
        ++m;
    }
}
