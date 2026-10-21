/*
 * A062900  Number and its reversal are both multiples of 8.
 * https://oeis.org/A062900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 925, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   rev_mod8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Search: a(n) is the n-th positive integer such that the number and its
 * decimal reversal are both multiples of 8.
 */
#include <stdint.h>
#include "../utils/rev_mod8.h"

seq_t a(idx_t n) {
    /* n >= 1 by OEIS offset */
    idx_t count = 0;
    for (idx_t k = 1;; ++k) {
        seq_t candidate = (seq_t)k * 8;  /* only multiples of 8 need checking */
        if (rev_mod8(candidate) == 0) {
            ++count;
            if (count == n)
                return candidate;
        }
    }
}
