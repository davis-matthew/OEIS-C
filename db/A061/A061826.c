/*
 * A061826  Multiples of 7 containing only even digits.
 * https://oeis.org/A061826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the nth multiple of 7 whose decimal digits are all even. */

#include <stdbool.h>

/* Test whether all decimal digits of x are even. */
static inline bool all_even_digits(seq_t x)
{
    if (x == 0)           /* 0 is considered to have only even digits */
        return true;

    while (x != 0) {
        seq_t dig   = x % 10;     /* least significant decimal digit   */
        if ((dig % 2) != 0)       /* odd digit found                    */
            return false;
        x /= 10;                  /* move to next digit                  */
    }
    return true;
}

/* Return the n-th term of the sequence A061826 (1‑based index). */
seq_t a(idx_t n)
{
    idx_t count = 0;        /* how many qualifying numbers have been seen */
    seq_t value = 0;        /* current multiple of 7 to test */

    for (;;) {
        if (all_even_digits(value)) {
            ++count;
            if (count == n)
                return value;
        }
        value += (seq_t)7;   /* advance to the next multiple of 7 */
    }
}
