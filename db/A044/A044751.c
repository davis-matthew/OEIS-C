/*
 * A044751  Numbers n such that string 3,8 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer whose decimal representation
 *            contains the substring "38" while the decimal representation of
 *            n+1 does not
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Check if a given number contains the digit pair 3 followed by 8
   (i.e., the substring "38") in its decimal representation. */
static bool has38(seq_t x)
{
    if (x < (seq_t)38)
        return false;
    seq_t prev = x % (seq_t)10;
    x /= (seq_t)10;
    while (x != 0)
    {
        seq_t cur = x % (seq_t)10;
        if (cur == (seq_t)3 && prev == (seq_t)8)
            return true;
        prev = cur;
        x /= (seq_t)10;
    }
    return false;
}

/* Return the nth term of OEIS sequence A044751.  Indexing starts at
   n = 1 (a(1) = 38).  If n == 0 the function returns 0 as a
   benign placeholder, although such an index is outside the official
   offset. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t found = 0;
    for (seq_t x = 0;; ++x)
    {
        if (has38(x) && !has38(x + 1))
        {
            ++found;
            if (found == n)
                return x;
        }
    }
}
