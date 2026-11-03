/*
 * A037342  Numbers whose base-2 and base-8 expansions have no digits in
 *          common.
 * https://oeis.org/A037342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer whose octal representation
 *            contains no digits 0 or 1 (i.e. its binary digits 0,1 never
 *            coincide with octal digits)
 * Range:     u8 n <= 114, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 78 terms through n = 967 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool octal_clean(idx_t v)
/* Returns true iff all octal digits of v are in {2,3,4,5,6,7} */
{
    while (v > 0) {
        unsigned d = v % 8;
        if (d < 2) /* digit 0 or 1 found */
            return false;
        v /= 8;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                   /* undefined for offset 0, harmless */

    idx_t count = 0;
    idx_t candidate = 1;

    while (1) {
        if (octal_clean(candidate)) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        ++candidate;
    }
}
