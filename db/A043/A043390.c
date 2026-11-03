/*
 * A043390  Numbers having two 5's in base 6.
 * https://oeis.org/A043390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the n-th positive integer whose base‑6 representation
   contains exactly two 5 digits. */
#include <stdbool.h>

static inline bool has_two_fives_u64(unsigned long long x)
{
    unsigned count = 0;
    while (x) {
        if ((x % 6ULL) == 5ULL) {
            if (++count > 2) return false;
        }
        x /= 6ULL;
    }
    return count == 2;
}

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
