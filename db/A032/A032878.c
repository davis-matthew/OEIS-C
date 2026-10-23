/*
 * A032878  Numbers whose base-7 representation Sum_{i=0..m} d(i)*7^i has d(0)
 *          > d(1) < d(2) > ...
 * https://oeis.org/A032878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   zigzag_base7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A032878: Numbers whose base‑7 representation alternates
   d0 > d1 < d2 > d3 < ….
   Computed by brute‑force search: test each integer for the
   zig‑zag property on its base‑7 digits. */
#include <stdbool.h>
#include "../utils/zigzag_base7.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t value = 1;
    while (true) {
        if (zigzag_base7(value)) {
            ++count;
            if (count == n) return value;
        }
        ++value;
    }
}
