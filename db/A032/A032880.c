/*
 * A032880  Numbers whose base-9 representation Sum_{i=0..m} d(i)*9^i has d(0)
 *          > d(1) < d(2) > ...
 * https://oeis.org/A032880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose base‑9 representation has alternating inequalities
 *            d0 > d1 < d2 > d3 < … ; this function enumerates the sequence
 *            and returns the nth term (1‑based)
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   valid_pattern
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/valid_pattern.h"

seq_t a(idx_t n) {
    seq_t candidate = 1;
    idx_t count = 0;
    while (true) {
        if (valid_pattern(candidate)) {
            ++count;
            if (count == n) return candidate;
        }
        ++candidate;
    }
}
