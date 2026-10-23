/*
 * A044716  Numbers n such that string 0,3 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044716:
   a(n) = the n-th positive integer whose decimal representation
   contains the substring "03" but the next integer (n+1) does not. */
#include <stdbool.h>

static inline bool contains_03(seq_t x)
{
    /* Test whether the decimal representation of x contains the
       substring "03".  We scan digits from least significant to
       most significant, checking each adjacent pair. */
    for (; x >= 10; x /= 10) {
        if ((x % 10) == 3 && ((x / 10) % 10) == 0)
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Find the n-th integer satisfying the condition. */
    idx_t i = 0;               /* candidate integer */
    idx_t count = 0;           /* number of matches found */

    for (;; ++i) {
        if (contains_03((seq_t)i) && !contains_03((seq_t)(i + 1))) {
            ++count;
            if (count == n)
                return (seq_t)i;
        }
    }
}
