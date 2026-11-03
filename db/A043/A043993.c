/*
 * A043993  Numbers k such that 6 and 7 occur juxtaposed in the base-9
 *          representation of k but not of k+1.
 * https://oeis.org/A043993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation scans integers in increasing order and returns the
 * nth integer k such that the base-9 representation of k contains two
 * adjacent digits 6 and 7 in either order, while the base-9 representation
 * of k+1 does not. */

#include <stdbool.h>

/* Return true if x (decimal) has two consecutive base‑9 digits 6 and 7
 * in either order. */
static bool has_adj_6_7(uint64_t x)
{
    if (x == 0) return false;

    uint8_t digits[17]; /* maximum digits needed for 64‑bit numbers in base‑9 */
    int len = 0;
    while (x > 0) {
        digits[len++] = (uint8_t)(x % 9);
        x /= 9;
    }

    for (int i = 1; i < len; ++i) {
        if ((digits[i] == 6 && digits[i-1] == 7) ||
            (digits[i] == 7 && digits[i-1] == 6))
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safe default for out of range */

    idx_t found = 0;

    for (uint64_t k = 0;; ++k) {
        if (has_adj_6_7(k) && !has_adj_6_7(k + 1)) {
            ++found;
            if (found == n) return (seq_t)k;
        }
    }
}
