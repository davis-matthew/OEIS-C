/*
 * A011540  Numbers that contain a digit 0.
 * https://oeis.org/A011540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   a(n) = nth nonnegative integer that contains at least one digit
 *            0
 * Range:     u8 n <= 44, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Check if x contains a zero digit.  Works for any integer type that
   overloads % and / with an integer literal.  Returns true for 0. */
static bool has_zero(seq_t x)
{
    if (x == (seq_t)0) return true;          /* 0 itself has a 0 digit */
    const seq_t ten = (seq_t)10;
    while (x != (seq_t)0) {
        if (x % ten == (seq_t)0) return true;
        x = x / ten;
    }
    return false;                            /* never reached */
}

/* Return the nth term (n >= 1) of the sequence A011540. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t cur = (seq_t)0;

    while (1) {
        if (has_zero(cur)) {
            ++count;
            if (count == n) return cur;
        }
        cur++;                               /* Next number to test */
    }
}
