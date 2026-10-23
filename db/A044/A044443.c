/*
 * A044443  Numbers n such that string 0,1 occurs in the base 4 representation
 *          of n but not of n+1.
 * https://oeis.org/A044443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computation: a(n) is the nth integer n such that the base‑4 representation
 * of n contains the substring "01" while the representation of n+1 does not.
 */

#include <stdbool.h>

static bool has01(seq_t v)
{
    if (v == 0)
        return false;

    unsigned prev = 4;               /* a sentinel that is never a base‑4 digit */
    while (v > 0)
    {
        unsigned digit = (unsigned)(v % 4);
        if (digit == 0 && prev == 1)
            return true;
        prev = digit;
        v = v / 4;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                   /* offset is 1; behaviour below is irrelevant */

    idx_t found = 0;
    seq_t m = 0;

    while (found < n)
    {
        ++m;
        if (has01(m) && !has01(m + 1))
            ++found;
    }
    return m;
}
