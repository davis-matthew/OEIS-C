/*
 * A044097  Numbers n such that string 4,0 occurs in the base-5 representation
 *          of n but not of n-1.
 * https://oeis.org/A044097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer whose base‑5 representation *
 *            contains the substring "40" while n-1 does not
 * Range:     u8 n <= 12, u16 n <= 1300, u32 n <= 1300, u64 n <= 1300,
 *            u128 n <= 1300, big any n
 * Helpers:   has_40
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_40.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t candidate = (seq_t)5;
    while (1) {
        if (has_40(candidate)) {
            if (!has_40(candidate - 1)) {
                ++count;
                if (count == n) return candidate;
            }
        }
        candidate += 5;
    }
}
