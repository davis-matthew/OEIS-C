/*
 * A032900  Numbers whose base-3 representation Sum_{i=0..m} d(i)*3^i has d(0)
 *          <= d(1) >= d(2) <= ...
 * https://oeis.org/A032900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   alt_base3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose base‑3 digits alternate:
   d0 <= d1 >= d2 <= d3 >= d4 ...  (OEIS A032900) */
#include <stdbool.h>
#include "../utils/alt_base3.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* offset is 1, so n==0 is unused */
    seq_t cand = 1;
    idx_t count = 0;
    while (true) {
        if (alt_base3(cand)) {
            ++count;
            if (count == n) return cand;
        }
        cand = cand + 1;
    }
}
