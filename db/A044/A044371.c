/*
 * A044371  Numbers n such that string 3,9 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044371: numbers n such that the decimal representation of n
 * contains the substring "39" but the representation of n-1 does not. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Return true iff the decimal representation of x contains "39". */
static bool contains_39(seq_t x)
{
    if (x == 0)
        return false;

    uint8_t digits[128];   /* enough for up to 128-bit numbers          */
    size_t len = 0;
    seq_t t = x;

    while (t > 0) {
        digits[len++] = (uint8_t)(t % 10);
        t /= 10;
    }

    for (int i = (int)len - 1; i >= 1; --i) {
        if (digits[i] == 3 && digits[i - 1] == 9)
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for index 0, return 0 */

    idx_t count = 0;
    seq_t candidate = 1;   /* start testing from 1 */

    for (;;) {
        if (contains_39(candidate) && !contains_39(candidate - 1)) {
            ++count;
            if (count == n)
                return candidate;
        }
        ++candidate;
    }
}
