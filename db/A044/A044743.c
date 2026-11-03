/*
 * A044743  Numbers n such that string 3,0 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th integer whose decimal representation contains the
 *            adjacent digits 3 followed by 0, while the representation of (n
 *            + 1) does not contain that substring
 * Range:     u8 n <= 3, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return true iff the decimal digit sequence of x contains "30". */
static bool contains30(seq_t x)
{
    seq_t prev = (seq_t)-1;           /* sentinel – not a decimal digit */
    while (x > 0) {
        seq_t d = x % 10;
        if (prev == 0 && d == 3)      /* found 3 followed by 0 */
            return true;
        prev = d;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; a(0) is undefined but harmless */
        return 0;

    seq_t x = 0;
    idx_t count = 0;

    for (;;) {
        ++x;                             /* examine the next integer */
        if (contains30(x) && !contains30(x + 1)) {
            ++count;
            if (count == n)
                return x;
        }
    }
}
