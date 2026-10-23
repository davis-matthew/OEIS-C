/*
 * A044378  Numbers n such that string 4,6 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers n such that the decimal representation of n contains the
 *            substring "46" but n-1 does not
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* True iff x contains the consecutive digits 4 followed by 6. */
static bool has46(seq_t x)
{
    /* If x has fewer than two digits, it cannot contain "46". */
    if (x < 10) return false;

    seq_t next_digit = x % 10;   /* Least significant digit */
    x /= 10;
    while (x > 0) {
        seq_t cur_digit = x % 10;
        if (cur_digit == 4 && next_digit == 6)
            return true;
        next_digit = cur_digit;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Sequence is defined for n >= 1; behaviour for n == 0 is irrelevant. */
    if (n == 0) return 0;

    seq_t count = 0;      /* Number of found terms so far. */
    seq_t x = 45;         /* Start from 46; x will be incremented first. */

    for (;; ++x) {
        if (has46(x) && !has46(x - 1)) {
            ++count;
            if (count == n)
                return x;
        }
    }
}
