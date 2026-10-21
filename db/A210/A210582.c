/*
 * A210582  Numbers whose first digit is the remainder of their division by
 *          the last digit (in base 10).
 * https://oeis.org/A210582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = numbers whose first digit equals the remainder of the
 *            division by the last digit (OEIS A210582)
 * Range:     u8 n <= 30, u16 n <= 4395, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset starts at 1 */

    seq_t val   = 1;   /* candidate number */
    idx_t count = 0;   /* how many terms found */

    while (1)
    {
        /* last digit */
        seq_t last = val % 10;
        if (last != 0)
        {
            /* first digit */
            seq_t temp = val;
            while (temp >= 10)
                temp /= 10;
            seq_t first = temp;

            /* check condition: first digit == val mod last digit */
            if (first == (val % last))
            {
                ++count;
                if (count == n)
                    return val;
            }
        }
        ++val;   /* next number to test */
    }
}
