/*
 * A044511  Numbers n such that string 4,4 occurs in the base 6 representation
 *          of n but not of n+1.
 * https://oeis.org/A044511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers n such that the base‑6 representation of n contains the
 * substring “44” but the representation of n+1 does not.
 * a(n) is the n‑th such number (OEIS A044511) */
#include <stdbool.h>

/* Test whether x in base 6 contains the consecutive digits 4,4. */
static bool has44(seq_t x)
{
    seq_t prev = x % 6;
    x /= 6;
    while (x > 0)
    {
        seq_t cur = x % 6;
        if (cur == 4 && prev == 4)
            return true;
        prev = cur;
        x /= 6;
    }
    return false;
}

seq_t a(idx_t n)
{
    seq_t cur = 1;
    idx_t count = 0;
    while (1)
    {
        if (has44(cur) && !has44(cur + 1))
        {
            ++count;
            if (count == n)
                return cur;
        }
        ++cur;
    }
}
