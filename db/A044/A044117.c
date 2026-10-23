/*
 * A044117  Numbers n such that string 2,3 occurs in the base 6 representation
 *          of n but not of n-1.
 * https://oeis.org/A044117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th integer n ≥ 1 such that the base‑6 digits of n
 *            contain the substring "23" while the base‑6 digits of n−1 do
 *            not. The function iteratively scans natural numbers, checks this
 *            property, and returns the n-th such value. All arithmetic is
 *            performed in the input type idx_t and the resu...
 * Range:     u8 n <= 8, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has_23_in_base6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_23_in_base6.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t num   = 1;

    while (1) {
        if (has_23_in_base6(num)) {
            if (!has_23_in_base6(num - 1)) {
                ++count;
                if (count == n)
                    return (seq_t)num;
            }
        }
        ++num;
    }
}
