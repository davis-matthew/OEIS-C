/*
 * A044258  Numbers n such that string 0,7 occurs in the base 9 representation
 *          of n but not of n-1.
 * https://oeis.org/A044258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th positive integer whose base‑9 representation *
 *            contains the substring "07" while the base‑9 representation of *
 *            the preceding integer does not
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return true iff x has the substring 0 followed by 7 in its base‑9
 * representation.  We scan digit by digit from least significant to
 * most significant.  The pattern is detected when the current digit
 * is 0 and the previous (more significant) digit was 7. */
static inline bool has07(idx_t x)
{
    int prev = -1;
    while (x > 0)
    {
        int d = (int)(x % 9);
        if (d == 0 && prev == 7)
            return true;
        prev = d;
        x /= 9;
    }
    return false;
}

/* a(idx_t n) returns the n-th term of sequence A044258.
 * It walks forward through the integers, keeping track of whether the
 * previous number exhibited the required substring.  When a number
 * has the substring and the previous one did not, we have found the
 * next term. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* unspecified for n < 1 */
    idx_t count = 0;       /* how many terms we have found */
    idx_t num = 1;         /* current integer to test */
    bool prevHas07 = false;/* did the previous integer have the substring? */

    while (count < n)
    {
        bool curHas07 = has07(num);

        if (curHas07 && !prevHas07)
        {
            ++count;
            if (count == n)
                return (seq_t)num;   /* found the n-th term */
        }

        prevHas07 = curHas07;
        ++num;
    }

    /* never reached */
    return 0;
}
