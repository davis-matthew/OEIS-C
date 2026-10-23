/*
 * A044772  Numbers n such that string 5,9 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044772: numbers whose decimal expansion contains the substring
 * "59" but whose successor does not.
 * The function searches integers sequentially. */

#include <stdbool.h>

/* Return true iff x has the decimal substring "59".  */
static bool has59(seq_t x)
{
    if (x < 10)            /* less than two digits cannot contain "59" */
        return false;

    seq_t last = x % 10;   /* least significant digit */
    x /= 10;               /* drop that digit */

    while (x > 0) {
        seq_t digit = x % 10;
        if (digit == 5 && last == 9)
            return true;   /* found "5" followed by "9" */
        last = digit;      /* keep the new last digit for next iteration */
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)            /* OEIS offset is 1; undefined for 0 */
        return 0;

    seq_t candidate = 1;   /* start searching from 1 */
    idx_t found = 0;       /* how many valid numbers found so far */

    while (found < n) {
        if (has59(candidate) && !has59(candidate + 1)) {
            ++found;
            if (found == n)
                return candidate;
        }
        ++candidate;       /* skip to next integer */
    }

    return 0;              /* unreachable; keeps compiler happy */
}
