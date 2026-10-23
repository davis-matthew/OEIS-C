/*
 * A044457  Numbers n such that string 3,3 occurs in the base 4 representation
 *          of n but not of n+1.
 * https://oeis.org/A044457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers n such that the base‑4 representation of n contains the substring
   "33" while the base‑4 representation of n+1 does not.  This implementation
   generates terms by a simple linear search.                                   */

#include <stdbool.h>

static bool contains33(seq_t x)
{
    int prev = -1;
    while (x > 0) {
        int digit = (int)(x % 4);   /* least significant base‑4 digit */
        if (prev == 3 && digit == 3)
            return true;
        prev = digit;
        x /= 4;
    }
    return false;
}

seq_t a(idx_t n)
{
    /*  n is 1‑based; for n < 1 the behaviour is undefined          */
    seq_t candidate = 1;
    idx_t   count    = 0;

    while (count < n) {
        if (contains33(candidate)) {
            seq_t next = candidate;
            next += 1;
            if (!contains33(next)) {
                ++count;
                if (count == n)
                    return candidate;
            }
        }
        candidate += 1;
    }

    /* fallback – should never be reached for valid n */
    return 0;
}
