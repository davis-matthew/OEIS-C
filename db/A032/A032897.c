/*
 * A032897  Numbers whose base-9 representation Sum_{i=0..m} d(i)*9^i has d(0)
 *          >= d(1) <= d(2) >= ...
 * https://oeis.org/A032897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   base9_zigzag
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n-th integer whose base‑9 digits satisfy
   d0 ≥ d1 ≤ d2 ≥ d3 … (least significant digit first). */

#include <stdbool.h>
#include "../utils/base9_zigzag.h"

seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t cand = 1;
    while (1) {
        if (base9_zigzag(cand)) {
            ++found;
            if (found == n) return cand;
        }
        ++cand;
    }
}
