/*
 * A044803  Numbers k such that string 9,0 occurs in the base 10
 *          representation of k but not of k+1.
 * https://oeis.org/A044803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth integer k such that the decimal representation of k
 *            contains the substring "90" while the representation of k+1 does
 *            not
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Helper: return true if x (in decimal) contains the substring "90" */
static bool has_90(seq_t x)
{
    /* Quick check for numbers < 90 */
    if (x < 90) return false;

    seq_t prev_digit = x % 10;   /* least significant digit */
    x /= 10;
    while (x > 0) {
        if (prev_digit == 0 && (x % 10) == 9) return true;
        prev_digit = x % 10;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (seq_t k = 0; ; ++k) {
        if (has_90(k) && !has_90(k + 1)) {
            ++count;
            if (count == n) return k;
        }
    }
}
