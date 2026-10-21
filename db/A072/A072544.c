/*
 * A072544  Numbers whose smallest decimal digit is also the initial digit.
 * https://oeis.org/A072544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 164, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A072544
   Numbers whose smallest decimal digit is also the initial digit.
   a(1) = 0.
   For n>1, a(n) is the nth integer (in increasing order) such that
   the leading (most‑significant) decimal digit equals the minimum
   decimal digit of the number. */

#include <stdbool.h>
#include <stdint.h>

/* Return true iff x satisfies the property. */
static bool good(seq_t x)
{
    /* Special case: 0 */
    if (x == 0)
        return true;

    /* Find leading digit */
    seq_t tmp = x;
    while (tmp >= 10)
        tmp /= 10;
    seq_t leading = tmp;

    /* Find minimum digit */
    seq_t min_digit = 9;
    tmp = x;
    while (tmp > 0) {
        seq_t d = tmp % 10;
        if (d < min_digit)
            min_digit = d;
        if (min_digit == 0)          /* can't get smaller than 0 */
            break;
        tmp /= 10;
    }

    return leading == min_digit;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return 0;          /* a(1) = 0 */

    idx_t count = 1;       /* we already have one valid number (0) */
    seq_t value = 1;       /* start checking from 1 */

    while (true) {
        if (good(value)) {
            ++count;
            if (count == n)
                return value;
        }
        ++value;
    }
}
