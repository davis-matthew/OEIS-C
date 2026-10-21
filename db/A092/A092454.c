/*
 * A092454  Numbers that either contain the digit 5 or are divisible by 5.
 * https://oeis.org/A092454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the n-th nonnegative integer whose decimal representation
 * contains a digit 5 or which is divisible by 5. */
/* The function iterates over natural numbers and counts those that
 * satisfy the criterion until it reaches the requested index n. */

#include <stdbool.h>

/* Check whether the decimal representation of x contains the digit 5 */
static bool contains5(seq_t x)
{
    while (x > 0) {
        if (x % 10 == 5) {
            return true;
        }
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t cnt = 0;
    seq_t i = 0;
    while (1) {
        /* The number 0 is counted as the first term. */
        if ((i % 5 == 0) || contains5(i)) {
            if (cnt == n) {
                return i;
            }
            ++cnt;
        }
        ++i;
    }
}
