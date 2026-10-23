/*
 * A044303  Numbers n such that string 5,7 occurs in the base 9 representation
 *          of n but not of n-1.
 * https://oeis.org/A044303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   has57_base9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers n such that the base‑9 representation of n contains
 * the directed pair 5,7 and the base‑9 representation of n-1 does not. */
#include <stdbool.h>
#include "../utils/has57_base9.h"

seq_t a(idx_t n)
{
    /* n is 1‑based. Search sequentially for the n‑th qualifying integer. */
    seq_t cur = 1;
    idx_t cnt = 0;
    while (1) {
        if (has57_base9(cur) && !has57_base9(cur - 1)) {
            ++cnt;
            if (cnt == n)
                return cur;
        }
        ++cur;
    }
}
