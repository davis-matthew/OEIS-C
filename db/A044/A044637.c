/*
 * A044637  Numbers n such that string 0,5 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 889, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has_05_in_base9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A044637
 * Numbers n such that the base‑9 representation of n contains the
 * substring "05" but the base‑9 representation of n+1 does not.
 */

#include <stdbool.h>
#include "../utils/has_05_in_base9.h"

/* Public function required by the build */
seq_t a(idx_t n)
{
    seq_t cur = 1;      /* start searching from 1 */
    idx_t found = 0;

    while (1) {
        if (has_05_in_base9(cur) && !has_05_in_base9(cur + 1)) {
            found++;
            if (found == n)
                return cur;
        }
        cur++;
    }
}
