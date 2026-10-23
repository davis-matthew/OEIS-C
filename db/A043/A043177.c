/*
 * A043177  Numbers k such that 5 and 6 occur juxtaposed in the base-8
 *          representation of k but not of k-1.
 * https://oeis.org/A043177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th positive integer k such that the octal
 *            representation of k contains the adjacent digits 5 and 6 in
 *            either order (i.e., “56” or “65”), but k−1 does not
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   has_adjacent56
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/has_adjacent56.h"

seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for n=0, return 0 as a safe default */
        return (seq_t)0;

    uint64_t count = 0;
    uint64_t k = 1;

    for (;;) {
        if (has_adjacent56(k)) {
            if (!has_adjacent56(k - 1)) {
                ++count;
                if (count == n)
                    return (seq_t)k;
            }
        }
        ++k;
    }
}
