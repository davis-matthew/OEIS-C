/*
 * A044059  Numbers n such that string 2,1 occurs in the base 3 representation
 *          of n but not of n-1.
 * https://oeis.org/A044059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A044059: a(n) is the nth positive integer whose base‑3 representation
 * contains the substring “21” while the representation of n‑1 does not.
 * The function computes this by scanning successive integers and testing
 * the stated condition. */

#include <stdbool.h>

static bool has21(seq_t x)
{
    /* base‑3: look for a 1 followed by a higher‑significance 2
     * (pattern 21).  Scan from least significant digit upward. */
    if (x == 0) return false;
    int prev = -1;
    while (x > 0)
    {
        int cur = (int)(x % 3);
        if (prev == 1 && cur == 2) return true;
        prev = cur;
        x /= 3;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* offset is 1, n==0 is undefined */

    idx_t count   = 0;
    seq_t candidate = 1;

    while (1)
    {
        if (has21(candidate))
        {
            seq_t prev = candidate - 1;
            if (!has21(prev))
            {
                ++count;
                if (count == n) return candidate;
            }
        }
        ++candidate;
    }
}
