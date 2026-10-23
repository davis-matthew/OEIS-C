/*
 * A039040  Numbers whose base-7 representation has the same number of 2's and
 *          3's.
 * https://oeis.org/A039040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth nonnegative integer whose base‑7 representation
 *            contains equal numbers of 2 and 3
 * Range:     u8 n <= 108, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   same_23
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/same_23.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined by OEIS, but safe */
    idx_t count = 0;
    seq_t candidate = 0;
    for (;; candidate++) {
        if (same_23(candidate)) {
            ++count;
            if (count == n) return candidate;
        }
    }
}
