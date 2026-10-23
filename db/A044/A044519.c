/*
 * A044519  Numbers n such that string 0,0 occurs in the base 7 representation
 *          of n but not of n+1.
 * https://oeis.org/A044519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has_consecutive_zeroes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: generate the n‑th number whose base‑7 representation contains
   the substring "00" but the representation of n+1 does not. */
#include <stdbool.h>
#include "../utils/has_consecutive_zeroes.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t x = 1;          /* candidate number */
    while (1) {
        if (has_consecutive_zeroes(x) && !has_consecutive_zeroes(x + 1)) {
            ++count;
            if (count == n) return x;
        }
        ++x;
    }
}
