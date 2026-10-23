/*
 * A030553  Position of n-th 4 in A030548.
 * https://oeis.org/A030553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Positions of the digit 4 in the concatenation of the base‑6
 * representations of the positive integers.
 *
 * a(n) = the position (1‑based) of the nth occurrence of the digit 4
 * in the infinite string
 *   "1 2 3 4 5 10 11 12 13 14 15 20 21 22 23 24 25 30 31 …"
 * where each number is written in base 6 and the digits are written
 * directly one after another.
 */

#include <stdint.h>

/* seq_t and idx_t are defined by the build environment. */
seq_t a(idx_t n)
{
    /* Count how many 4's have been seen. */
    idx_t count = 0;

    /* Current position in the stream (1‑based). */
    seq_t pos = 0;

    /* Buffer for the base‑6 digits of the current number.
     * log_6(2^64) < 22, so 64 slots are more than enough. */
    idx_t digits[64];

    for (idx_t num = 1; ; ++num)
    {
        /* Convert num to base‑6 and store digits in reverse order. */
        idx_t x    = num;
        idx_t len  = 0;

        if (x == 0)
        {
            digits[len++] = 0;
        }
        else
        {
            while (x > 0)
            {
                digits[len++] = x % 6;
                x /= 6;
            }
        }

        /* Emit digits from most significant to least significant. */
        for (idx_t i = len; i > 0; --i)
        {
            seq_t d = digits[i - 1];
            ++pos;                 /* advance to next position */

            if (d == 4)
            {
                ++count;
                if (count == n)
                    return pos;
            }
        }
    }
}
