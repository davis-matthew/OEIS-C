/*
 * A121034  Multiples of 14 containing a 14 in their decimal representation.
 * https://oeis.org/A121034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 260, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A121034: returns the n-th multiple of 14 containing the decimal
   substring "14".  The algorithm scans successive multiples of 14 and
   tests each for the pattern 1 followed by 4. */

#include <stdbool.h>

/* Test whether a decimal integer has the substring "14".
   Scans from least significant digit; if a digit 4 is followed
   by a digit 1 in more significant position the pattern is found. */
static bool contains_14(seq_t x)
{
    seq_t prev = x % 10;
    x /= 10;
    while (x > 0) {
        seq_t d = x % 10;
        if (prev == 4 && d == 1) return true;
        prev = d;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safeguard, offset is 1 */
    seq_t candidate = 14;          /* first multiple of 14 */
    idx_t  count = 1;              /* number of valid multiples found so far */

    while (1) {
        if (contains_14(candidate)) {
            if (count == n) return candidate;
            ++count;
        }
        candidate += 14;           /* next multiple of 14 */
    }
}
