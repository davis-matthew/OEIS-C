/*
 * A044542  Numbers n such that string 3,2 occurs in the base 7 representation
 *          of n but not of n+1.
 * https://oeis.org/A044542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer whose base‑7 representation
 *            contains the substring “32” but whose successor (n+1) does not
 *            contain that substring
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Test whether base‑7 representation of x contains the
   adjacent digits 3 (higher) followed by 2 (lower). */
static inline bool contains32(seq_t x)
{
    if (x == 0)
        return false;

    /* process least‑significant digit first */
    seq_t prev = x % 7;
    x /= 7;
    while (x != 0)
    {
        seq_t cur = x % 7;
        /* pattern 3,2 appears when low digit is 2
           and the next (higher) digit is 3        */
        if (prev == 2 && cur == 3)
            return true;
        prev = cur;
        x /= 7;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t candidate = 0;

    for (;;)
    {
        candidate++;                   /* consider next integer */
        if (contains32(candidate) && !contains32(candidate + 1))
        {
            ++count;
            if (count == n)
                return candidate;
        }
    }
}
