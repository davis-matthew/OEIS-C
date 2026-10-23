/*
 * A044230  Numbers n such that string 5,3 occurs in the base 8 representation
 *          of n but not of n-1.
 * https://oeis.org/A044230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A044230 – Numbers n such that the octal representation of n
 * contains the substring "53" while the representation of n-1 does not. */
/* This implementation uses a simple linear search: it scans successive
 * integers and collects those that satisfy the condition until the
 * requested index n is reached.  The algorithm is O(n * log₈ n) and
 * runs well below the 2‑second limit for n up to about 10 000. */

#include <stdbool.h>

static bool has53(seq_t value)
{
    /* Return true iff the octal representation of value contains the
     * consecutive digits 5 followed by 3. */
    if (value == 0) return false;
    /* The maximum number of octal digits for a 64‑bit value is 21
     * (since 8^21 > 2^63).  A buffer of 32 digits safely covers all
     * supported seq_t sizes (including arbitrary‑precision). */
    unsigned digit[32];
    int len = 0;
    while (value != 0)
    {
        digit[len++] = (unsigned)(value % 8);
        value /= 8;
    }
    /* Scan from the most significant digit downwards. */
    for (int i = len - 2; i >= 0; --i)
    {
        if (digit[i + 1] == 5 && digit[i] == 3)
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* According to OEIS, a(1) = 43, a(2) = 107, etc. */
    if (n == 0) return 0;          /* undefined; return 0 for safety */
    seq_t count = 0;
    seq_t current = 0;
    while (count < n)
    {
        ++current;                  /* examine next integer */
        if (has53(current))
        {
            /* n-1 can be zero when current==1; has53(0) returns false,
             * so the condition is satisfied when current=1 and the
             * representation contains "53" (which it never does). */
            if (!has53(current - 1))
            {
                ++count;
                if (count == n)
                    return current;
            }
        }
    }
    return current;    /* unreachable but keeps the compiler happy */
}
