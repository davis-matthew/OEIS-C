/*
 * A044087  Numbers n such that string 2,0 occurs in the base 5 representation
 *          of n but not of n-1.
 * https://oeis.org/A044087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   has_sub20
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the n‑th positive integer whose base‑5 representation contains
   the substring “20” but whose predecessor does not. */

#include <stdbool.h>
#include "../utils/has_sub20.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t m = 0;
    while (count < n) {
        m = m + 1;
        if (has_sub20(m) && !has_sub20(m - 1))
            ++count;
    }
    return m;
}
