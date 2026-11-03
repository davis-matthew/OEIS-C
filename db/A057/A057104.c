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
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth positive integer that contains at least one decimal digit 8 or 9 */

#include <stdbool.h>

static inline bool has8or9(seq_t x)
{
    while (x > (seq_t)0) {
        int d = (int)(x % (seq_t)10);
        if (d == 8 || d == 9)
            return true;
        x /= (seq_t)10;
    }
    return false;
}

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
