/*
 * A102701  Non-"Ding!Bong!" numbers: positive numbers which are not a
 *          positive linear combination of 5's and 7's.
 * https://oeis.org/A102701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Non-Ding!Bong! numbers: numbers not of the form 5·x + 7·y (x,y ≥ 0).
   All integers >23 are representable, so the sequence stops after 12 terms. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    idx_t count = 0;
    for (idx_t i = 1; ; ++i) {
        bool rep = false;
        for (idx_t x = 0; x <= i / 5; ++x) {
            idx_t rem = i - 5 * x;
            if (rem % 7 == 0) {
                rep = true;
                break;
            }
        }
        if (!rep) {
            ++count;
            if (count == n)
                return (seq_t)i;
        }
        if (i > 23)          /* after 23 all numbers are representable */
            break;
    }
    return (seq_t)0;
}
