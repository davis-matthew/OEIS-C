/*
 * A044025  Numbers k such that 3 and 9 occur juxtaposed in the base-10
 *          representation of k but not of k+1.
 * https://oeis.org/A044025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has_adjacent_39_93
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A044025:  Numbers k such that 3 and 9 occur juxtaposed in the base-10
 * representation of k but not of k+1.
 * For a given index n (with offset 1), a(n) is the n-th such number.
 * The implementation scans integers until the n-th qualifying one is found.
 */

#include <stdbool.h>
#include "../utils/has_adjacent_39_93.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* behavior below offset is irrelevant */

    idx_t found = 0;
    for (idx_t k = 1; ; ++k) {
        seq_t x  = (seq_t)k;
        seq_t nx = x + 1;
        if (has_adjacent_39_93(x) && !has_adjacent_39_93(nx)) {
            ++found;
            if (found == n)
                return x;
        }
    }
}
