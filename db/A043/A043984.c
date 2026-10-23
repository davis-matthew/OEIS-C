/*
 * A043984  Numbers k such that 3 and 7 occur juxtaposed in the base-9
 *          representation of k but not of k+1.
 * https://oeis.org/A043984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k such that the digits 3 and 7 occur adjacent
   in the base‑9 representation of k, but not in the base‑9 representation
   of k+1.  The sequence is a(1)=34, a(2)=66, …                     */

#include <stdbool.h>

/* Return true if the base‑9 representation of x contains two
   adjacent digits 3 and 7 in either order. */
static bool has_adjacent_3_7(idx_t x)
{
    if (x < 9)          /* single digit cannot contain a pair */
        return false;

    int prev = x % 9;   /* least significant digit */
    x /= 9;
    while (x > 0) {
        int curr = x % 9;   /* next higher digit */
        if ((curr == 3 && prev == 7) || (curr == 7 && prev == 3))
            return true;
        prev = curr;
        x /= 9;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)         /* offset is 1; behaviour for n=0 is unspecified */
        return 0;

    idx_t count = 0;    /* how many qualifying numbers found so far */
    idx_t k = 0;        /* candidate number */

    while (count < n) {
        ++k;
        if (has_adjacent_3_7(k) && !has_adjacent_3_7(k + 1))
            ++count;
    }

    return (seq_t)k;
}
