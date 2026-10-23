/*
 * A044132  Numbers n such that string 5,0 occurs in the base 6 representation
 *          of n but not of n-1.
 * https://oeis.org/A044132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   return the nth positive integer whose base‑6 representation
 *            contains the substring "50" while the representation of the
 *            previous number does not. The implementation simply scans
 *            integers until the nth qualifying value is found
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool has50(seq_t x)
{
    seq_t temp = x;
    bool prev_zero = false;
    while (temp > 0) {
        seq_t d = temp % 6;
        if (prev_zero && d == 5) return true;
        prev_zero = (d == 0);
        temp /= 6;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* n is at least 1 (offset 1). */
    seq_t candidate = 1;
    idx_t found = 0;
    while (found < n) {
        seq_t prev = candidate;
        prev -= 1;                 /* candidate - 1 (valid for candidate >= 1) */
        if (has50(candidate) && !has50(prev)) {
            found++;
            if (found == n)
                return candidate;
        }
        candidate++;                /* step to next integer */
    }
    /* Unreachable for valid input, but satisfy compiler. */
    return 0;
}
