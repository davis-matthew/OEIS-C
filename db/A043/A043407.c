/*
 * A043407  Numbers whose base-7 representation contains exactly three 3's.
 * https://oeis.org/A043407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   hasThreeThrees
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043407: nth positive integer whose base-7 representation contains exactly three 3's. */
#include <stdint.h>
#include "../utils/hasThreeThrees.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n=0, return 0 by convention */
    seq_t candidate = 1;
    idx_t found = 0;
    while (1) {
        if (hasThreeThrees(candidate)) {
            ++found;
            if (found == n) return candidate;
        }
        ++candidate;
    }
}
