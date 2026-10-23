/*
 * A032959  Numbers whose base-9 representation Sum_{i=0..m} d(i)*9^(m-i) has
 *          even d(i) for all odd i.
 * https://oeis.org/A032959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 147, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a032959_valid
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A032959: Nth integer whose base‑9 digits at odd positions (from most
   significant) are even. */
#include <stdbool.h>
#include "../utils/a032959_valid.h"

seq_t a(idx_t n)
{
    seq_t cur = 1;
    idx_t count = 0;

    while (1) {
        if (a032959_valid(cur)) {
            ++count;
            if (count == n)
                return cur;
        }
        ++cur;
    }
}
