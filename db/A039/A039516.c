/*
 * A039516  Numbers whose base-12 representation has the same nonzero number
 *          of 2's and 5's.
 * https://oeis.org/A039516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth integer whose base‑12 representation has the same
 *            nonzero count of 2's and 5's
 * Range:     u8 n <= 4, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   same_25
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/same_25.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    idx_t found = 0;
    seq_t num = (seq_t)1;
    for (;; ++num)
    {
        if (same_25(num))
        {
            ++found;
            if (found == n)
                return num;
        }
    }
}
