/*
 * A057104  The non-octal numbers: numbers containing an 8 or 9 (they cannot
 *          be mistaken for octal numbers).
 * https://oeis.org/A057104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   has8or9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth positive integer that contains at least one decimal digit 8 or 9 */

#include <stdbool.h>
#include "../utils/has8or9.h"

seq_t a(idx_t n)
{
    /*  n must be >= 1  */
    idx_t found = 0;
    seq_t cur = (seq_t)8;          /* first integer that can contain 8 or 9 */
    while (found < n) {
        if (has8or9(cur)) {
            ++found;
            if (found == n)
                return cur;
        }
        ++cur;
    }
    /* unreachable but keep compiler happy */
    return (seq_t)0;
}
