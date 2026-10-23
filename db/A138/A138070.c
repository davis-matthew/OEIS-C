/*
 * A138070  Triangle read by rows: row n lists the successive digits of
 *          A135697(n), the palindromic number formed from the reflected
 *          decimal expansion of Pi.
 * https://oeis.org/A138070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl, base
 * Formula:   compute the digit at position n in the triangle formed by the
 *            palindromes A135697(r); each row r contains r digits, and the
 *            palindrome is built from the first ceil(r/2) digits of Pi
 *            (including the integer part 3). The implementation finds the row
 *            and the position within that row, then selects the...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_from_str.h"

static const uint8_t PI_DIGITS[71] = {
    3,1,4,1,5,9,2,6,5,3,5,8,9,7,9,3,2,3,8,4,
    6,2,6,4,3,3,8,3,2,7,9,5,0,2,8,8,4,1,9,7,
    1,6,9,3,9,9,3,7,5,1,0,5,8,2,0,9,7,4,9,4,
    4,5,9,2,3,0,7,8,1,6,4
};

static const char *const DIG_STR[10] = {
    "0","1","2","3","4","5","6","7","8","9"
};

seq_t a(idx_t n)
{
    /* Find the row (r) such that the first r rows contain at least n
       terms.  sum is the total number of terms in rows 1..r. */
    idx_t r = 0;
    idx_t sum = 0;
    while (sum + (r + 1) < n) {
        r++;
        sum += r;
    }
    idx_t row = r + 1;          /* actual row number */
    idx_t pos = n - sum;        /* position within this row (1‑based) */

    /* Determine index of the Pi digit that appears at this position. */
    idx_t mid = (row + 1) / 2;
    idx_t idx;
    if (pos <= mid)
        idx = pos - 1;         /* first half including middle */
    else
        idx = row - pos;       /* mirrored second half */

    return seq_from_str(DIG_STR[PI_DIGITS[idx]]);
}
