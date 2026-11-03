/*
 * A039110  Numbers whose base-9 representation has the same number of 6's and
 *          8's.
 * https://oeis.org/A039110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 164, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 9913 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS sequence A039110:
   Numbers whose base‑9 representation contains an equal number of 6's and 8's.
   The function returns the n‑th such number (1‑based indexing, a(1)=0). */

#include <stdbool.h>          /* for bool */

/* Helper: true iff the base‑9 digits of 'value' have the same count of 6 and 8 */
static bool balanced_in_base9(idx_t value)
{
    /* 0 has no 6's or 8's – counts are equal. */
    if (value == 0) {
        return true;
    }

    idx_t tmp   = value;
    idx_t c6 = 0, c8 = 0;

    while (tmp != 0) {
        int digit = tmp % 9;          /* digit is 0…8, isolated with % 9 */
        if (digit == 6) {
            ++c6;
        } else if (digit == 8) {
            ++c8;
        }
        tmp /= 9;
    }
    return c6 == c8;
}

/* External function required by the framework */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;          /* safety; offset starts at 1 */
    }

    idx_t count = 0;
    for (idx_t val = 0; ; ++val) {
        if (balanced_in_base9(val)) {
            ++count;
            if (count == n) {
                return (seq_t)val;
            }
        }
    }
}
