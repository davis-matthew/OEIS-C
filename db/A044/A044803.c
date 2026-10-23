/*
 * A044803  Numbers k such that string 9,0 occurs in the base 10
 *          representation of k but not of k+1.
 * https://oeis.org/A044803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth integer k such that the decimal representation of k
 *            contains the substring "90" while the representation of k+1 does
 *            not
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   has_90
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_90.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (seq_t k = 0; ; ++k) {
        if (has_90(k) && !has_90(k + 1)) {
            ++count;
            if (count == n) return k;
        }
    }
}
