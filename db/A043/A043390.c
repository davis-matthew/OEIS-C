/*
 * A043390  Numbers having two 5's in base 6.
 * https://oeis.org/A043390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   has_two_fives_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the n-th positive integer whose base‑6 representation
   contains exactly two 5 digits. */
#include <stdbool.h>
#include "../utils/has_two_fives_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    unsigned long long candidate = 1;
    idx_t found = 0;
    while (1) {
        if (has_two_fives_u64(candidate)) {
            ++found;
            if (found == n) {
                return (seq_t)candidate;
            }
        }
        ++candidate;
    }
}
