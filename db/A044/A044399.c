/*
 * A044399  Numbers n such that string 6,7 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers n such that decimal representation contains "67" but n-1
 *            does not
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   contains_67
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/contains_67.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t cur = 1;
    while (1) {
        if (contains_67(cur) && !contains_67(cur - 1)) {
            ++count;
            if (count == n)
                return cur;
        }
        ++cur;
    }
}
