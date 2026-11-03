/*
 * A045097  Numbers whose base-4 representation contains exactly two 1's and
 *          two 2's.
 * https://oeis.org/A045097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer whose base‑4 representation contains
 *            exactly two 1's and two 2's
 * Range:     u8 n <= 6, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Helper: test whether x has exactly two digits = 1 and two digits = 2 in base‑4. */
static inline bool has_two_one_two_two(uint64_t x)
{
    uint8_t c1 = 0, c2 = 0;
    while (x) {
        uint8_t d = x & 3;   /* x % 4 */
        if (d == 1) {
            ++c1;
            if (c1 > 2) return false;
        } else if (d == 2) {
            ++c2;
            if (c2 > 2) return false;
        }
        x >>= 2;            /* x / 4 */
    }
    return (c1 == 2 && c2 == 2);
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1, but return 0 safely */

    idx_t found = 0;
    uint64_t val = 0;

    for (val = 1;; ++val) {
        if (has_two_one_two_two(val)) {
            ++found;
            if (found == n)
                return (seq_t)val;
        }
    }
}
