/*
 * A044739  Numbers n such that string 2,6 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth integer such that the decimal representation of n
 *            contains the substring "26" but the decimal representation of
 *            n+1 does not. The implementation searches sequentially from 26
 *            upward until n terms are found
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   has26
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has26.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (seq_t x = (seq_t)26; ; ++x) {
        if (has26(x) && !has26(x + 1)) {
            ++count;
            if (count == n) return x;
        }
    }
}
