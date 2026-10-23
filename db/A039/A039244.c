/*
 * A039244  Numbers whose base-12 representation has the same number of 3's
 *          and 9's.
 * https://oeis.org/A039244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   has_equal_3_and_9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039244: a(n) is the n‑th nonnegative integer whose base-12
 * representation contains an equal number of digits 3 and 9.  */
#include <stdbool.h>
#include "../utils/has_equal_3_and_9.h"

seq_t a(idx_t n)
{
    /* a(1) = 0 */
    if (n == 1) return (seq_t)0;

    seq_t val = 1;
    idx_t count = 1;          /* we have already counted 0 */

    while (1)
    {
        if (has_equal_3_and_9(val))
        {
            ++count;
            if (count == n)
                return val;
        }
        ++val;   /* next integer to test */
    }
}
