/*
 * A032895  Numbers whose base-7 representation Sum_{i=0..m} d(i)*7^i has d(0)
 *          >= d(1) <= d(2) >= ...
 * https://oeis.org/A032895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   alt_base7_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: returns the n‑th positive integer whose base‑7 digits
   satisfy the alternating inequality pattern
   d(0) ≥ d(1) ≤ d(2) ≥ d(3) ≤ ...   (d(0) is the least
   significant digit).  This sequence is OEIS A032895. */

#include <stdbool.h>
#include "../utils/alt_base7_ok.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t val = 0;
    while (1) {
        ++val;
        if (alt_base7_ok(val)) {
            ++count;
            if (count == n) return val;
        }
    }
}
