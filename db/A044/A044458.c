/*
 * A044458  Numbers n such that string 0,0 occurs in the base 5 representation
 *          of n but not of n+1.
 * https://oeis.org/A044458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   has_consecutive_zeros_in_base5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: numbers whose base‑5 representation contains "00" but (n+1) does not */
#include <stdbool.h>
#include "../utils/has_consecutive_zeros_in_base5.h"

seq_t a(idx_t n)
{
    /* Find the n-th integer that has "00" in its base‑5 representation
       while the next integer does not. */
    idx_t count = 0;
    seq_t m = 0;

    while (count < n) {
        ++m; // test next integer

        if (has_consecutive_zeros_in_base5(m) &&
            !has_consecutive_zeros_in_base5(m + 1)) {
            ++count;
            if (count == n)
                return m;
        }
    }
    /* Should never reach here */
    return 0;
}
