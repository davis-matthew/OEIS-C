/*
 * A011534  Numbers that contain a 4.
 * https://oeis.org/A011534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 6878, u32 n <= 6878, u64 n <= 6878,
 *            u128 n <= 6878, big any n
 * Validated: standard, pass, 69 terms through n = 6382 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A011534: a(n) = nth positive integer whose decimal representation contains the digit 4. */
#include <stdbool.h>

static bool contains4(idx_t x)
{
    while (x > 0) {
        if (x % 10 == 4)
            return true;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t i = 1;
    while (1) {
        if (contains4(i)) {
            count++;
            if (count == n)
                return (seq_t)i;
        }
        i++;
    }
}
