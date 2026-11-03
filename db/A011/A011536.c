/*
 * A011536  Numbers that contain a 6.
 * https://oeis.org/A011536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 3439, u32 n <= 3439, u64 n <= 3439,
 *            u128 n <= 3439, big any n
 * Validated: standard, pass, 69 terms through n = 2602 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: find the nth integer containing digit '6'. */
#include <stdbool.h>

/* helper: true iff x contains the decimal digit 6 */
static bool contains_digit6(seq_t x)
{
    while (x > (seq_t)0) {
        if ((x % (seq_t)10) == (seq_t)6)
            return true;
        x /= (seq_t)10;
    }
    return false;
}

seq_t a(idx_t n)
{
    seq_t val = 0;
    idx_t count = 0;

    while (count < n) {
        val++;                      /* next integer to test          */
        if (contains_digit6(val))
            ++count;                /* found a number containing 6   */
    }
    return val;
}
